//
//  PianoGameMainScene.cpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/10/24.
//

#include "PianoGameMainScene.h"


#include "MainScene.h"
#include "SimpleAudioEngine.h"

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "PianoGame.hpp"

USING_NS_CC;
using namespace  rapidjson;

#define COUNTDOWN 3.0f
#define MOVE_TIME 0.5f
#define DETERMINE_TIME 0.2f

Scene * PianoGameMainScene::CreateScene()
{
    return create();
}

std::string PianoGameMainScene::s_backgroundMusic = "PianoGame/MusicScore/doll.mp3";
std::string PianoGameMainScene::s_gameJson = "PianoGame/MusicScore/doll.json";

bool PianoGameMainScene::init()
{
    if( !BaseScene::init() )
    {
        return false;
    }
    
    m_musicTime = 0.0f;
    m_currPlayTime = 0.0f;
    m_playing = false;
    m_countDownNumber = 0;
    m_backgroundMusicPlaying = false;
    m_centerControlScale = 1.0f;
    
    m_showBollIndex = 0;
    m_touchToneIndex = 0;
    
    auto t_visibleSizeHalf = Director::getInstance()->getVisibleSize() * 0.5f;
    Vec2 t_origin = Director::getInstance()->getVisibleOrigin();
    
    TexturePacker::PianoGame::addSpriteFramesToCache();
    
    //    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect( "Piano/A.wav" );
    
    //    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic( s_backgroundMusic.c_str() );
    
    //load json
    std::string t_fileStr = FileUtils::getInstance()->getStringFromFile( s_gameJson );
    
    
    Document t_readdoc;
    
    t_readdoc.Parse<0>( t_fileStr.c_str() );
    
    if( t_readdoc.HasParseError() )
    {
        printf( "GetParseError %d \n", t_readdoc.GetParseError() );
    }
    
    int t_speed = t_readdoc["speed"].GetInt();
    auto & t_data = t_readdoc["data"];
    double t_time = 0.0f;
    
    double t_toneTime = 60.0f / (double)t_speed;
    
    auto & t_keys = t_readdoc["keys"];
    
    for( int i = 0; i < t_keys.Capacity() ; ++i ){
        
        // std::stringstream t_sstr;
        // t_sstr << "key_" << i << ".png";
        // Director::getInstance()->getTextureCache()->addImage( t_sstr.str() );
        // t_sstr.str("");
        // t_sstr << "keyTouch_" << i << ".png";
        // Director::getInstance()->getTextureCache()->addImage( t_sstr.str() );
        

        std::string t_tone = t_keys[i].GetString();
        m_sequeueTone.push_back( t_tone );
        
        
        std::stringstream t_audio;
        t_audio << "PianoGame/Piano/" << t_tone << ".wav";
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect( t_audio.str().c_str() );
        
        t_audio.str("");
        t_audio << "PianoGame/Piano/" << t_tone << "+1.wav";
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect( t_audio.str().c_str() );
        
        t_audio.str("");
        t_audio << "PianoGame/Piano/" << t_tone << "+2.wav";
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect( t_audio.str().c_str() );
        
    }
    
    m_sequeueTone.push_back( m_sequeueTone[0] );
    
    
    for( int i = 0; i < t_data.Capacity(); ++i )
    {
        auto & t_item = t_data[i];
        
        double t_note = t_item[ "note" ].GetDouble();
        double t_special = t_item[ "special" ].GetDouble();
        std::vector< std::string > t_tones;
        
        if( t_item[ "tone" ].GetType() == kStringType )
        {
            t_tones.push_back( t_item[ "tone" ].GetString() );
        }
        
        if( t_item[ "tone" ].GetType() == kArrayType )
        {
            for( int n = 0; n < t_item[ "tone" ].Capacity(); ++n )
            {
                t_tones.push_back( t_item[ "tone" ][n].GetString() );
            }
        }
        
        Tone tone{
            t_time,
            t_tones
        };
        
        m_musicScore.push_back( tone );
        
        t_time += t_toneTime / t_note * t_special;
    }
    
    
    auto t_centerPoit = Vec2( t_origin.x + t_visibleSizeHalf.width, t_origin.y + t_visibleSizeHalf.height );
    
    auto t_background = Sprite::create( "PianoGameBackground.png" );
    auto t_backgroundSizeHalf = t_background->getContentSize() * 0.5f;
    t_background->setScale( t_visibleSizeHalf.width / t_backgroundSizeHalf.width , t_visibleSizeHalf.height / t_backgroundSizeHalf.height );
    t_background->setPosition( t_centerPoit );
    
    addChild( t_background );
    
    auto t_BGRound = Sprite::create("PianoGameBGRound.png");
    auto t_BGRoundSizeHalf = t_BGRound->getContentSize() * 0.5f;
    
    auto t_BGRoundScale = t_visibleSizeHalf.height / ( t_BGRoundSizeHalf.height + 10.0f );
    
    t_BGRound->setScale( t_BGRoundScale, t_BGRoundScale );
    t_BGRound->setPosition( t_centerPoit );
    
    addChild( t_BGRound );
    
    auto t_centerControl = TexturePacker::PianoGame::createCenterControlSprite();
    auto t_centerControlSizeHalf = t_centerControl->getContentSize() * 0.5f;
    m_centerControlPosition = t_centerPoit + Vec2( -3.0f, 3.0f );
    
    m_centerControlScale = t_visibleSizeHalf.height / ( t_centerControlSizeHalf.height + 25.0f );
    
    t_centerControl->setScale( m_centerControlScale );
    t_centerControl->setPosition( m_centerControlPosition );
    
    addChild( t_centerControl );
    
    auto t_keyBottom = t_centerControlSizeHalf.width * m_centerControlScale * 0.4546f * 0.5f;
    auto t_drumR = (t_centerControlSizeHalf.height * m_centerControlScale) - t_keyBottom;
    
    auto t_minus = TexturePacker::PianoGame::createMinusSprite();
    t_minus->setScale( m_centerControlScale );
    t_minus->setPosition( m_centerControlPosition + Vec2( cos( PI / 180.0f * 105.0f ) * t_drumR, sin( PI / 180.0f * 105.0f) * t_drumR ) );
    addChild( t_minus );
    
    auto t_plus = TexturePacker::PianoGame::createPlusSprite();
    t_plus->setScale( m_centerControlScale );
    t_plus->setPosition( m_centerControlPosition + Vec2( cos( PI / 180.0f * 75.0f ) * t_drumR, sin( PI / 180.0f * 75.0f) * t_drumR ) );
    addChild( t_plus );
    
    m_startPos = Vec2( cos( PI / 180.0f * 150.0f) * t_drumR, sin( PI / 180.0f * 150.0f ) * t_drumR );
    
    m_angleOffset = 165.0f;
    
    for( int i = 0; i < 8; ++i ){
        
        std::stringstream sstr;
        
        sstr << "key_" << i << ".png";
        
        float t_angele = PI / 180.0f * ( m_angleOffset + (float)i * 30.0f );
        
        
        printf("-------------->");
        auto t_texture = Director::getInstance()->getTextureCache()->getTextureForKey( sstr.str() );
        
        auto t_key = Sprite::createWithTexture( t_texture );
         printf("-------------->");
        t_key->setScale( m_centerControlScale );
        auto t_keyOffset =  Vec2( cos( t_angele ) * t_drumR, sin( t_angele ) * t_drumR );
        t_key->setPosition( m_centerControlPosition + t_keyOffset );
        
        m_sequeueDirection.push_back( t_keyOffset );
        
        auto t_shadow = Sprite::createWithTexture( Director::getInstance()->getTextureCache()->getTextureForKey( "shadow.png" ) );
        t_shadow->setScale( m_centerControlScale );
        t_shadow->setPosition( m_centerControlPosition + t_keyOffset + Vec2( 5.0f, -6.0f ) );
        
        addChild( t_shadow );
        
        addChild( t_key );
    }
    
    
    MenuItemFont * t_startPause = MenuItemFont::create( "start", [=](Ref * p_target){
        Label * t_label = (Label *)((MenuItemFont*)p_target)->getLabel();
        if( !m_playing ){
            m_playing = true;
            m_currPlayTime = 0.0f;
            t_label->setString( "pause" );
            
        }else{
            m_playing = false;
            t_label->setString( "start" );
            
            CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
            CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
        }
    });
    
    t_startPause->setFontSizeObj( 30 );
    t_startPause->setColor( Color3B( 255, 0, 0 ) );
    t_startPause->setPosition( Vec2( t_origin.x + 60.0f, t_origin.y + t_visibleSizeHalf.height * 2.0f - 40.0f ) );
    
    
    MenuItemFont * t_back = MenuItemFont::create( "back", [=](Ref * p_target){
        Director::getInstance()->replaceScene( MainScene::CreateScene() );
    });
    
    t_back->setFontSizeObj( 30 );
    t_back->setColor( Color3B( 255, 0, 0 ) );
    t_back->setPosition( Vec2( t_origin.x + 60.0f, t_origin.y + t_visibleSizeHalf.height * 2.0f - 80.0f ) );
    
    auto t_menu = Menu::create( t_startPause, t_back, NULL );
    t_menu->setPosition(Vec2::ZERO);
    addChild( t_menu );
    
    
    m_countDownLabel = Label::createWithTTF( "", "fonts/Marker Felt.ttf", 60 );
    
    m_countDownLabel->setPosition( t_centerPoit );
    m_countDownLabel->setColor( Color3B( 255, 0, 0 ) );
    addChild( m_countDownLabel );
    
    
    //    auto t_v1 = Vec2( cos( PI / 180.0f * 165.0f ) * t_drumR, sin( PI / 180.0f * 165.0f ) * t_drumR );
    //    auto t_v2 = Vec2( cos( PI / 180.0f * -15.0f ) * t_drumR, sin( PI / 180.0f * -15.0f ) * t_drumR );
    ////    acos( t_v1.dot( t_v2 ) / t_v1.getAngle(<#const Vec2 &other#>) )
    //
    //    printf( "-------:%f \n", t_v1.getAngle( t_v2 ) / PI * 180.0f );
    
    
    auto t_touchListener = EventListenerTouchAllAtOnce::create();
    t_touchListener->setEnabled( true );
    
    t_touchListener->onTouchesBegan = [=]( const std::vector<Touch*>& touches, Event *event ){
        
        for( auto t_touche : touches ){
            auto t_location = t_touche->getLocation();
            
            auto t_distance = t_location.getDistance( m_centerControlPosition );
            
            if( t_distance > t_drumR + t_keyBottom || t_distance < t_drumR - t_keyBottom )
            {
                continue;
            }
            
            t_location = t_location - m_centerControlPosition;
            
            auto t_angle = m_startPos.getAngle( t_location ) / PI * 180.0f;
            int t_touchIndex = -1;
            if( t_angle >= 0.0f )
            {
                t_touchIndex = (int)floor(t_angle / 30.0f);
            }else if( t_angle <= -120.0f ){
                t_touchIndex = 7 - (int)floor( abs( ( (t_angle + 120.0f) / 30.0f ) ) );
            }else{
                continue;
            }
            
            std::stringstream t_sstr;
            
            t_sstr << "Piano/" << m_sequeueTone[t_touchIndex] << "+" << ( t_touchIndex < 7 ? "1" : "2" ) << ".wav";
            
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect( t_sstr.str().c_str() );
            
        }
    };
    
    t_touchListener->onTouchesEnded = []( const std::vector<Touch*>& touches, Event *event ){
        //        printf( "------> onTouchesEnded %d", touches.size() );
    };
    
    t_touchListener->onTouchesMoved = []( const std::vector<Touch*>& touches, Event *event ){
        // sprintf( "------> onTouchesMoved %d", touches.size() );
    };
    
    t_touchListener->onTouchesCancelled = []( const std::vector<Touch*>& touches, Event *event ){
        //        printf( "------> onTouchesCancelled %d", touches.size() );
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority( t_touchListener, this );
    
    schedule( schedule_selector( PianoGameMainScene::update ) );
    
    return true;
}


void PianoGameMainScene::update( float p_delta )
{
    bool t_updateMusicTime = true;
    if( !m_playing )
    {
        return;
    }
    
    if( m_currPlayTime < COUNTDOWN )
    {
        t_updateMusicTime = false;
        m_currPlayTime += p_delta;
        
        
        int t_countDown = (int)floor( COUNTDOWN - m_currPlayTime ) + 1;
        
        //倒计时时间
        if( t_countDown < 4 && t_countDown != m_countDownNumber )
        {
            m_countDownNumber = t_countDown;
            
            std::stringstream t_sstr;
            if( m_countDownNumber > 0 ){
                t_sstr << m_countDownNumber;
            }
            
            m_countDownLabel->setString( t_sstr.str() );
        }
        
        
        //完成倒计时
        if( m_currPlayTime >= COUNTDOWN )
        {
            m_currPlayTime = 4.0f;
            
            t_updateMusicTime = true;
            
            auto t_audioEnigne = CocosDenshion::SimpleAudioEngine::getInstance();
            
            t_audioEnigne->resumeAllEffects();
            t_audioEnigne->resumeBackgroundMusic();
        }
    }else{
        m_musicTime += p_delta;
    }
    
    //没有准备完之前不计算
    if( !t_updateMusicTime )
    {
        return;
    }
    
    if( !m_backgroundMusicPlaying && m_musicTime >= 0.0f )
    {
        //开始播放背景音乐
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic( s_backgroundMusic.c_str() );
        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume( 0.4f );
        m_backgroundMusicPlaying = true;
    }
    
    if( m_showBollIndex < m_musicScore.size() && m_musicScore[m_showBollIndex].time <= m_musicTime + MOVE_TIME )
    {
        //需要显示下一个音符
        for( auto item : m_musicScore[m_showBollIndex].tone )
        {
            
            std::string t_strTone = item;
            auto t_find = item.find( "+" );
            if( t_find != std::string::npos )
            {
                t_strTone = t_strTone.substr(0, t_find);
            }
            
            auto t_findRes = std::find( m_sequeueTone.begin(), m_sequeueTone.end(), t_strTone );
            if( t_findRes == m_sequeueTone.end() )
            {
                continue;
            }
            int t_index = (int)( t_findRes - m_sequeueTone.begin() );
            std::stringstream t_sstr;
            t_sstr << "key_" << t_index << ".png";
            
            auto t_texture = Director::getInstance()->getTextureCache()->getTextureForKey( t_sstr.str() );
            
            auto t_sprite = Sprite::createWithTexture( t_texture );
            t_sprite->setScale( m_centerControlScale );
            addChild( t_sprite );
            
            GameTone t_gameTone{
                m_musicScore[m_showBollIndex].time,
                t_index,
                t_sprite
            };
            m_gameTones.push_back( t_gameTone );
            //            printf( "-------%d \n", t_index );
        }
        m_showBollIndex++;
    }
    
    
    
    for( int i = 0; i < m_gameTones.size(); ++i )
    {
        auto item = m_gameTones[i];
        auto t_timeOffset = m_musicTime - item.time + MOVE_TIME;
        
        if( m_musicTime - item.time >= DETERMINE_TIME )
        {
            removeChild( item.ball );
            m_gameTones.erase( m_gameTones.begin() + i );
            --i;
            continue;
        }
        item.ball->setPosition( m_centerControlPosition + t_timeOffset / MOVE_TIME * m_sequeueDirection[item.index] );
    }
}



PianoGameMainScene::~PianoGameMainScene( void )
{
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
    
    TexturePacker::PianoGame::removeSpriteFramesFromCache();
}
