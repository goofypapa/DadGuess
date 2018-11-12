//
//  CadenceGameScene.cpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/11/7.
//

#include "CadenceGameScene.h"
#include "CadenceMain.hpp"
#include "ui/CocosGUI.h"
#include "Common.h"
#include "CadenceGameMainScene.h"
#include "AudioEngine.h"

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"


#define MOVE_TIME 2.0f
#define FAULT_TOLERANT_TIME 0.1f


USING_NS_CC;
using namespace  rapidjson;
using namespace cocos2d::ui;
using namespace experimental;

#define SHOW_TOUCH_ARE false

const char * CadenceGameScene::sm_audioList[] = {
    "Cadence/Djembe/B.wav",
    "Cadence/Djembe/T.wav",
    "Cadence/Djembe/S.wav"
};

CadenceGameScene * CadenceGameScene::CreateScene( const std::string p_musicDir, const std::string p_musicName )
{
    auto t_result = create();
    
    if( !t_result->initWithMusic( p_musicDir, p_musicName ) )
    {
        t_result->release();
        t_result = nullptr;
    }
    
    return t_result;
}

bool CadenceGameScene::init( void )
{
    if( !BaseScene::init() )
    {
        return false;
    }
    
    m_selectLevel = 0;
    m_playing = false;
    
    return true;
}

bool CadenceGameScene::initWithMusic( const std::string p_musicDir, const std::string p_musicName )
{
    auto t_visibleSizeHalf = Director::getInstance()->getVisibleSize() * 0.5f;
    Vec2 t_origin = Director::getInstance()->getVisibleOrigin();
    
    std::stringstream t_sstr;
    t_sstr << "Cadence/MusicScore/" << p_musicDir;
    m_musicScoreDir = t_sstr.str();
    
    t_sstr.str("");
    t_sstr << m_musicScoreDir << "/bgm.wav";
    m_bgmPath = t_sstr.str();
    
    auto t_background = Sprite::create( "Cadence/background_1.png" );
    auto t_backgroundSizeHalf = t_background->getContentSize() * 0.5f;
    t_background->setScale( t_visibleSizeHalf.width / t_backgroundSizeHalf.width , t_visibleSizeHalf.height / t_backgroundSizeHalf.height );
    t_background->setPosition( Vec2( t_origin.x + t_visibleSizeHalf.width, t_origin.y + t_visibleSizeHalf.height ) );
    addChild( t_background );
    
    
    auto t_backButton = Button::create( "back.png", "back.png", "", Button::TextureResType::PLIST );
    auto t_backButtonSizeHalf = t_backButton->getContentSize() * 0.5f;
    t_backButton->setPosition( Vec2( t_origin.x + t_backButtonSizeHalf.width * adaptation() + 10.0f, t_origin.y + t_visibleSizeHalf.height * 2.0f - t_backButtonSizeHalf.height * adaptation() - 10.0f ) );
    t_backButton->setScale( adaptation() );
    
    touchAnswer( t_backButton, []( Ref * p_target ){
        Director::getInstance()->replaceScene( CadenceGameMainScene::create() );
    }, adaptation() * 1.1f, adaptation());
    
    addChild( t_backButton );
    
    m_judgeSprite = TexturePacker::CadenceMain::createPerfectSprite();
    m_judgeSprite->setScale( adaptation() );
    m_judgeSpritePos = Vec2( t_origin.x + t_visibleSizeHalf.width, t_origin.y + t_visibleSizeHalf.height );
    m_judgeSprite->setPosition( m_judgeSpritePos );
    m_judgeSprite->setOpacity( 0 );
    addChild( m_judgeSprite, 90 );
    
    
    m_perfectLabel = Label::createWithSystemFont( "perfect: ", "Marker Felt", 20 );
    m_perfectLabel->setColor( Color3B( 255, 0, 0 ) );
    
    m_perfectLabel->setPosition( Vec2( t_origin.x + 30.0f, t_origin.y + t_visibleSizeHalf.height * 2.0f - 80.0f ) );
    m_perfectLabel->setAnchorPoint( Vec2( 0.0f, 0.5f ) );
    
    addChild( m_perfectLabel, 1000 );
    
    
    m_excellentLabel = Label::createWithSystemFont( "good: ", "Marker Felt", 20 );
    m_excellentLabel->setColor( Color3B( 255, 0, 0 ) );
    
    m_excellentLabel->setPosition( Vec2( t_origin.x + 30.0f, t_origin.y + t_visibleSizeHalf.height * 2.0f - 100.0f ) );
    m_excellentLabel->setAnchorPoint( Vec2( 0.0f, 0.5f ) );
    
    addChild( m_excellentLabel, 1000 );
    
    
    m_leakLabel = Label::createWithSystemFont( "miss: ", "Marker Felt", 20 );
    m_leakLabel->setColor( Color3B( 255, 0, 0 ) );
    
    m_leakLabel->setPosition( Vec2( t_origin.x + 30.0f, t_origin.y + t_visibleSizeHalf.height * 2.0f - 120.0f ) );
    m_leakLabel->setAnchorPoint( Vec2( 0.0f, 0.5f ) );
    
    addChild( m_leakLabel, 1000 );
    
    //
    auto t_drum = TexturePacker::CadenceMain::createGame_pic_guSprite();
    auto t_drumSizeHalf = t_drum->getContentSize() * 0.5f;
    auto t_drumScale = t_visibleSizeHalf.height * 0.7f / t_drumSizeHalf.height;
    auto t_drumScaleSizeHalf = t_drumSizeHalf * t_drumScale;
    
    t_drum->setScale( t_drumScale );
    t_drum->setPosition( Vec2( t_origin.x + t_visibleSizeHalf.width, t_origin.y + t_drumScaleSizeHalf.height ) );
    addChild( t_drum );
    
    float t_hideHeight = t_drumScaleSizeHalf.height * 0.24f;
    
    float t_drumCenter = ( t_drumScaleSizeHalf.height + t_hideHeight * 0.5f ) - t_hideHeight;
    
    m_lightCenterp = TexturePacker::CadenceMain::createGame_pic_light_center_pSprite();
    m_lightCenterp->setScale( t_drumScale );
    m_lightCenterp->setPosition( Vec2( t_origin.x + t_visibleSizeHalf.width, t_origin.y + t_drumCenter ) );
    m_lightCenterp->setOpacity( 0 );
    addChild( m_lightCenterp );

    m_lightMidp = TexturePacker::CadenceMain::createGame_pic_light_mid_pSprite();
    m_lightMidp->setScale( t_drumScale );
    auto t_lightMidpSizeHalf = m_lightMidp->getContentSize() * 0.5f * m_lightMidp->getScale();
    m_lightMidp->setPosition( Vec2( t_origin.x + t_visibleSizeHalf.width, t_origin.y + t_lightMidpSizeHalf.height ) );
    m_lightMidp->setOpacity( 0 );
    addChild( m_lightMidp );

    m_lightOutp = TexturePacker::CadenceMain::createGame_pic_light_out_pSprite();
    m_lightOutp->setScale( t_drumScale );
    auto t_lightOutpSizeHalf = m_lightOutp->getContentSize() * 0.5f * m_lightOutp->getScale();
    m_lightOutp->setPosition( Vec2( t_origin.x + t_visibleSizeHalf.width, t_origin.y + t_lightOutpSizeHalf.height ) );
    m_lightOutp->setOpacity( 0 );
    addChild( m_lightOutp );

    m_lightCentern = TexturePacker::CadenceMain::createGame_pic_light_center_nSprite();
    m_lightCentern->setScale( t_drumScale );
    m_lightCentern->setPosition( Vec2( t_origin.x + t_visibleSizeHalf.width, t_origin.y + t_drumCenter ) );
    m_lightCentern->setOpacity( 0 );
    addChild( m_lightCentern );

    m_lightMidn = TexturePacker::CadenceMain::createGame_pic_light_mid_nSprite();
    m_lightMidn->setScale( t_drumScale );
    auto t_lightMidnSizeHalf = m_lightMidn->getContentSize() * 0.5f * m_lightMidn->getScale();
    m_lightMidn->setPosition( Vec2( t_origin.x + t_visibleSizeHalf.width, t_origin.y + t_lightMidnSizeHalf.height ) );
    m_lightMidn->setOpacity( 0 );
    addChild( m_lightMidn );

    m_lightOutn = TexturePacker::CadenceMain::createGame_pic_light_out_nSprite();
    m_lightOutn->setScale( t_drumScale );
    auto t_lightOutnSizeHalf = m_lightOutn->getContentSize() * 0.5f * m_lightOutn->getScale();
    m_lightOutn->setPosition( Vec2( t_origin.x + t_visibleSizeHalf.width, t_origin.y + t_lightOutnSizeHalf.height ) );
    m_lightOutn->setOpacity( 0 );
    addChild( m_lightOutn );
    
    m_tipsSpriteList.push_back( m_lightOutn );
    m_tipsSpriteList.push_back( m_lightMidn );
    m_tipsSpriteList.push_back( m_lightCentern );
    
    static float s_info[][3] = {
        { t_drumCenter, t_drumScaleSizeHalf.width * 0.305f, 0.83f },
        { t_drumCenter * 0.88f, t_drumScaleSizeHalf.width * 0.7f, 0.73f },
        { t_drumCenter * 0.74f, t_drumScaleSizeHalf.width * 0.91f, 0.76f },
    };
    
    
    if( SHOW_TOUCH_ARE )
    {
        auto t_canvas = DrawNode::create();
        t_canvas->setPosition( t_origin );

        for( int i = 0; i < sizeof( s_info ) / sizeof( float[3] ); ++i )
        {
            t_canvas->drawCircle( Vec2( t_visibleSizeHalf.width, s_info[i][0] ), s_info[i][1], 360.0f, 60, true, 1.0f, s_info[i][2], Color4F( 1.0f, 0.0f, 0.0f, 1.0f ) );
        }
        
        addChild( t_canvas );
    }
    
    
    //
    for( auto i = 0; i < 3; ++i )
    {
        auto t_guide = TexturePacker::CadenceMain::createGame_pic_guide_blueSprite();
        t_guide->setScale( t_drumScale );
        auto t_guideSizeHalf = t_guide->getContentSize() * 0.5f;
        m_runwaySpace = t_guideSizeHalf.height * 4.0f * t_guide->getScale();
        auto t_pos = Vec2( t_origin.x + t_visibleSizeHalf.width, t_origin.y + t_visibleSizeHalf.height * 1.72f - i * m_runwaySpace );
        
        t_guide->setPosition( t_pos );
        
        addChild( t_guide );
        
        Sprite * t_remind = nullptr;
        
        switch ( i ) {
            case 0:
                t_remind = TexturePacker::CadenceMain::createGame_pic_guide_remind_purpleSprite();
                break;
            case 1:
                t_remind = TexturePacker::CadenceMain::createGame_pic_guide_remind_yellowSprite();
                break;
            case 2:
                t_remind = TexturePacker::CadenceMain::createGame_pic_guide_remind_pinkSprite();
                break;
            default:
                break;
        }
        t_remind->setPosition( t_pos );
        t_remind->setScale( t_drumScale );
        t_remind->setOpacity( 0 );
        addChild( t_remind );
        
        m_tipsSpriteList.push_back( t_remind );
        
        if( i != 1 )
        {
            continue;
        }
        m_startPointPos = Vec2( t_pos.x + t_guideSizeHalf.width * t_drumScale, t_pos.y + m_runwaySpace );
        m_runwayLength = t_guideSizeHalf.width * t_drumScale * 2.0f;
        m_endLinePos = t_guideSizeHalf.width * 1.82f * t_drumScale;
        
        auto t_finishLine = TexturePacker::CadenceMain::createGame_pic_endSprite();
        t_finishLine->setScale( t_drumScale );
        t_finishLine->setPosition( Vec2(  m_startPointPos.x - m_endLinePos, t_pos.y ) );
        addChild( t_finishLine, 10 );
    }
    
    
    auto t_selectLevel = Layer::create();
    addChild( t_selectLevel, 100 );
    
    auto t_mask = DrawNode::create();
    t_mask->drawSolidRect( Vec2::ZERO, t_visibleSizeHalf * 2.0f, Color4F( 0.0f, 0.0f, 0.0f, 1.0f ) );
    t_mask->setPosition( t_origin );
    t_mask->setOpacity( 120 );
    t_selectLevel->addChild( t_mask );
    
    float t_selectLevelScale = adaptation() * 1.5f;
    
    auto t_ordinary = Button::create( TexturePacker::CadenceMain::ordinary, TexturePacker::CadenceMain::ordinary, "", TextureResType::PLIST );
    t_ordinary->setScale( t_selectLevelScale );
    auto t_ordinaryScaleSizeHalf = t_ordinary->getContentSize() * t_ordinary->getScale() * 0.5f;
    auto t_ordinaryPos = Vec2( t_origin.x + t_visibleSizeHalf.width, t_origin.y + t_visibleSizeHalf.height );
    t_ordinary->setPosition( t_ordinaryPos );
    t_ordinary->setTag( 1 );
    t_selectLevel->addChild( t_ordinary );
    
    auto t_simple = Button::create( TexturePacker::CadenceMain::simple_selected, TexturePacker::CadenceMain::simple_selected, "", TextureResType::PLIST );
    t_simple->setScale( t_selectLevelScale );
    t_simple->setPosition( t_ordinaryPos - Vec2( t_ordinaryScaleSizeHalf.width * 2.0f + 20.0f, 0.0f ) );
    t_simple->setTag( 0 );
    t_selectLevel->addChild( t_simple );
    
    auto t_difficulty = Button::create( TexturePacker::CadenceMain::difficulty, TexturePacker::CadenceMain::difficulty, "", TextureResType::PLIST );
    t_difficulty->setScale( t_selectLevelScale );
    t_difficulty->setPosition( t_ordinaryPos + Vec2( t_ordinaryScaleSizeHalf.width * 2.0f + 20.0f, 0.0f ) );
    t_difficulty->setTag( 2 );
    t_selectLevel->addChild( t_difficulty );
    
    auto t_touchBack = [=]( Ref * p_target ){
        int t_tag = ((Button *)p_target)->getTag();
        
        if( m_selectLevel == t_tag )
        {
            return;
        }
        
        Button * t_items[] = {
            t_simple, t_ordinary, t_difficulty
        };
        
        std::string t_imgList[][2] = {
            {TexturePacker::CadenceMain::simple, TexturePacker::CadenceMain::simple_selected},
            {TexturePacker::CadenceMain::ordinary, TexturePacker::CadenceMain::ordinary_selected},
            {TexturePacker::CadenceMain::difficulty, TexturePacker::CadenceMain::difficulty_selected},
        };
        
        for( int i = 0; i < sizeof( t_items ) / sizeof( Button * ); ++i )
        {
            if( m_selectLevel == i )
            {
                t_items[i]->loadTextures( t_imgList[i][0] , t_imgList[i][0], "", TextureResType::PLIST );
            }
            
            if( t_tag == i )
            {
                t_items[i]->loadTextures( t_imgList[i][1] , t_imgList[i][1], "", TextureResType::PLIST );
            }
        }
        
        m_selectLevel = t_tag;
    };
    
    touchAnswer( t_ordinary, t_touchBack, t_selectLevelScale * 1.1f, t_selectLevelScale );
    touchAnswer( t_simple, t_touchBack, t_selectLevelScale * 1.1f, t_selectLevelScale );
    touchAnswer( t_difficulty, t_touchBack, t_selectLevelScale * 1.1f, t_selectLevelScale );
    
    auto t_soundName = Label::createWithSystemFont( p_musicName, "", 18 );
    auto t_soundNameSizeHalf = t_soundName->getContentSize() * 0.5f;
    auto t_soundNamePos = Vec2( t_ordinaryPos.x, t_ordinaryPos.y + t_soundNameSizeHalf.height + t_ordinaryScaleSizeHalf.height + 20.0f );
    t_soundName->setPosition( t_soundNamePos );
    t_selectLevel->addChild( t_soundName );
    
    auto t_soundNameLeft = TexturePacker::CadenceMain::createSoundNameLeftSprite();
    t_soundNameLeft->setScale( t_selectLevelScale );
    auto t_soundNameLeftScaleSizeHalf = t_soundNameLeft->getContentSize() * 0.5f * t_soundNameLeft->getScale();
    t_soundNameLeft->setPosition( t_soundNamePos - Vec2( t_soundNameSizeHalf.width + t_soundNameLeftScaleSizeHalf.width + 10.0f, 0.0f ) );
    t_selectLevel->addChild( t_soundNameLeft );
    
    auto t_soundNameRight = TexturePacker::CadenceMain::createSoundNameRightSprite();
    t_soundNameRight->setScale( t_selectLevelScale );
    auto t_soundNameRightScaleSizeHalf = t_soundNameRight->getContentSize() * 0.5f * t_soundNameRight->getScale();
    t_soundNameRight->setPosition( t_soundNamePos + Vec2( t_soundNameSizeHalf.width + t_soundNameRightScaleSizeHalf.width + 10.0f, 0.0f ) );
    t_selectLevel->addChild( t_soundNameRight );
    
    auto t_startButton = Button::create( TexturePacker::CadenceMain::startButton, TexturePacker::CadenceMain::startButton, "", TextureResType::PLIST );
    t_startButton->setScale( t_selectLevelScale );
    auto t_startButtonScaleSizeHalf = t_startButton->getContentSize() * t_startButton->getScale() * 0.5f;
    t_startButton->setPosition( Vec2( t_ordinaryPos.x, t_ordinaryPos.y - t_ordinaryScaleSizeHalf.height - t_startButtonScaleSizeHalf.height - 20.0f ) );
    t_selectLevel->addChild( t_startButton );
    
    touchAnswer( t_startButton, [=]( Ref * p_target ){
        
        startGame();
        
        t_selectLevel->runAction( ActionFloat::create( 0.1f, 1.0f, 0.0f, [=]( const float t_dt ){
            t_selectLevel->setOpacity( (GLbyte)floor( 255.0f * t_dt ) );
            if( t_dt <= 0.0f )
            {
                t_selectLevel->setVisible( false );
            }
        } ) );
        
        
        
    }, t_selectLevelScale * 1.1f, t_selectLevelScale );
    
    
    Sprite * s_lightList[] = {
        m_lightCenterp, m_lightMidp, m_lightOutp
    };
    
    auto t_touchListener = EventListenerTouchOneByOne::create();
    
    t_touchListener->onTouchBegan = [=]( Touch * p_touch, Event * p_event )->bool{
        
        auto t_pos = p_touch->getLocation() - t_origin;
        
        for( int i = 0; i < sizeof( s_info ) / sizeof( float[3] ); ++i )
        {
            auto t_center = Vec2( t_visibleSizeHalf.width, s_info[i][0] );
            auto t_maxR = s_info[i][1];
            auto t_minR = t_maxR * s_info[i][2];
            auto t_distance = t_pos.distance( t_center );
            if( t_distance > t_maxR )
            {
                continue;
            }
            
            auto t_offsetPos = t_pos - t_center;
            auto t_proportion = fmod( abs( t_offsetPos.getAngle() / PI * 2.0f ), 2.0f );
            t_proportion = t_proportion <= 1.0f ? t_proportion : 2.0f - t_proportion;
            
            if( t_distance <= t_maxR * ( 1.0f - t_proportion ) + t_minR * t_proportion )
            {
                s_lightList[i]->stopAllActions();
                
                s_lightList[i]->runAction( Sequence::create( ActionFloat::create( 0.06f, 0.0, 1.0f, [=]( const float p_dt)  {
                    if( p_dt <= 0.0f ){
                        s_lightList[i]->setOpacity( 255 );
                    }
                }), ActionFloat::create( 0.02f, 1.0f, 0.0f, [=]( const float p_dt)  {
                    s_lightList[i]->setOpacity( (GLubyte)floor( 255.0f * p_dt ) );
                }) , NULL) );
                
//                m_tipsSpriteList[2 - i]->runAction( Sequence )
                
                toneTouched( 2 - i );
                break;
            }
        }
        
        return true;
    };
    
    t_touchListener->onTouchEnded = [=]( Touch * p_touch, Event * p_event ){
        
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority( t_touchListener, this );
    
    schedule( schedule_selector( CadenceGameScene::update ) );
    
    return true;
}

void CadenceGameScene::loadMusicSource()
{
    m_musicScore.clear();
    
    Document t_readdoc;
    
     AudioEngine::preload( m_bgmPath );
    
    std::stringstream t_sstr;
   
    t_sstr << m_musicScoreDir << "/musicSource_" << m_selectLevel + 1 << ".json";
    
    //load json
    std::string t_fileStr = FileUtils::getInstance()->getStringFromFile( t_sstr.str() );
    
    t_readdoc.Parse<0>( t_fileStr.c_str() );
    
    if( t_readdoc.HasParseError() )
    {
        printf( "GetParseError %d \n", t_readdoc.GetParseError() );
    }
    
    int t_speed = t_readdoc["speed"].GetInt();
    auto & t_data = t_readdoc["data"];
    double t_time = 0.0f;
    
    double t_toneTime = 60.0f / (double)t_speed;
    
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
        
        t_time += t_toneTime / t_note / t_special;
    }
}

void CadenceGameScene::startGame( void )
{
    loadMusicSource();
    m_musicTime = 0.0f;
    m_accumulativeTime = 0.0f;
    m_backgroundMusicCalibration = false;
    m_backgroundMusicHandle = AudioEngine::play2d( m_bgmPath );
    
    timeval t_time;
    gettimeofday( &t_time, NULL );
    m_startTime = (double)t_time.tv_sec + ( (double)t_time.tv_usec / 1000000.0f );
    
    m_playing = true;
}


void CadenceGameScene::update( float p_delta )
{
    if( !m_playing )
    {
        return;
    }
    
    timeval _currTime;
    gettimeofday( &_currTime, NULL );
    
    double t_currTime = (double)_currTime.tv_sec + (double)(_currTime.tv_usec / 1000000.0f);
    
    m_musicTime = t_currTime - m_startTime + m_accumulativeTime;
    
    
    if( !m_backgroundMusicCalibration )
    {
        if( abs( m_musicTime - 1.0f ) <= 0.01f )
        {
            m_backgroundMusicCalibration = true;
            m_accumulativeTime += AudioEngine::getCurrentTime( m_backgroundMusicHandle ) - m_musicTime;
            
            printf( "---------> m_accumulativeTime: %f \n", m_accumulativeTime );
        }
    }
    
    static std::string s_toneOrder = "BTS";
    while( true )
    {
        if( m_showBollIndex < m_musicScore.size() && m_musicTime + MOVE_TIME >= m_musicScore[m_showBollIndex].time )
        {
            for( auto t_item : m_musicScore[m_showBollIndex].tone )
            {
                GameTone t_gameTone;
                auto t_findRes = s_toneOrder.find( t_item );
                
                if( t_findRes == std::string::npos )
                {
                    continue;
                }

                t_gameTone.index = (int)t_findRes;
                
                t_gameTone.time = m_musicScore[m_showBollIndex].time;
                t_gameTone.tone = t_item;
                t_gameTone.tips = false;
                
                std::string t_frameName;
                switch ( t_gameTone.index ) {
                    case 0:
                        t_frameName = TexturePacker::CadenceMain::game_pic_ball_blue;
                        break;
                    case 1:
                        t_frameName = TexturePacker::CadenceMain::game_pic_ball_yellow;
                        break;
                    case 2:
                        t_frameName = TexturePacker::CadenceMain::game_pic_ball_pink;
                        break;
                    default:
                        break;
                }
                auto t_spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName( t_frameName );
                auto t_sprite = Sprite::createWithSpriteFrame( t_spriteFrame );
                t_sprite->setScale( adaptation() );
                t_sprite->setPosition( m_startPointPos - Vec2( 0.0f, m_runwaySpace * t_gameTone.index ) );

                addChild( t_sprite, 20 );

                t_gameTone.ball = t_sprite;
                m_gameTones.push_back( t_gameTone );
            }
            
            ++m_showBollIndex;
            continue;
        }
        break;
    }
    
    
    
    for( int i = 0; i < m_gameTones.size(); ++i )
    {
        auto & t_item  = m_gameTones[i];
        
        if( !t_item.tips && t_item.time - m_musicTime <= FAULT_TOLERANT_TIME + 0.1f )
        {
            m_tipsSpriteList[t_item.index]->stopAllActions();
            m_tipsSpriteList[t_item.index]->runAction( ActionFloat::create(0.1f, 0.0f, 1.0f, [=]( const float p_dt ){
                m_tipsSpriteList[t_item.index]->setOpacity( (int)floor( 255.0f * p_dt ) );
                m_tipsSpriteList[t_item.index + 3]->setOpacity( (int)floor( 255.0f * p_dt ) );
            } ) );

            t_item.tips = true;
        }

        if( t_item.tips && m_musicTime - t_item.time >= FAULT_TOLERANT_TIME - 0.1f )
        {
            m_tipsSpriteList[t_item.index]->stopAllActions();
            m_tipsSpriteList[t_item.index]->runAction( ActionFloat::create(0.1f, 1.0f, 0.0f, [=]( const float p_dt ){
                m_tipsSpriteList[t_item.index]->setOpacity( (int)floor( 255.0f * p_dt ) );
                m_tipsSpriteList[t_item.index + 3]->setOpacity( (int)floor( 255.0f * p_dt ) );
            } ) );
            t_item.tips = false;
        }
        
        if( m_musicTime - t_item.time >= FAULT_TOLERANT_TIME )
        {
            removeChild( t_item.ball );
            m_gameTones.erase( m_gameTones.begin() + i );
            --i;
            changeJudge( Leak );
            continue;
        }
        
        float t_pos = m_endLinePos - ( t_item.time - m_musicTime ) / MOVE_TIME * m_endLinePos;
        t_item.ball->setPosition( Vec2( m_startPointPos.x - t_pos , m_startPointPos.y - m_runwaySpace * t_item.index ) );
    }

}


void CadenceGameScene::toneTouched( const int p_index )
{
    int t_toneIndex = -1;
    float t_time;
    for( int i = 0; i < m_gameTones.size(); ++i )
    {
        t_time = abs( m_gameTones[i].time - m_musicTime );
        if( m_gameTones[i].index == p_index && t_time <= FAULT_TOLERANT_TIME )
        {
            t_toneIndex = i;
            break;
        }
    }
    
    if( t_toneIndex > -1 )
    {
        if( t_time <= 0.025f )
        {
            changeJudge( Perfect );
        }else{
            changeJudge( Excellent );
        }
        
        
        if( m_gameTones[t_toneIndex].tips )
        {
            int t_index = m_gameTones[t_toneIndex].index;
            
            m_tipsSpriteList[t_index]->stopAllActions();
            m_tipsSpriteList[t_index]->runAction( ActionFloat::create(0.1f, 1.0f, 0.0f, [=]( const float p_dt ){
                m_tipsSpriteList[t_index]->setOpacity( (int)floor( 255.0f * p_dt ) );
                m_tipsSpriteList[t_index + 3]->setOpacity( (int)floor( 255.0f * p_dt ) );
            } ) );
            m_gameTones[t_toneIndex].tips = false;
        }

        removeChild( m_gameTones[t_toneIndex].ball );
        m_gameTones.erase( m_gameTones.begin() + t_toneIndex );
        
    }else{
        //miss
        changeJudge( Leak );
    }
    
    AudioEngine::play2d( sm_audioList[2 - p_index] );
}


void CadenceGameScene::changeJudge( const Judge p_judge )
{
    m_judgeSprite->stopAllActions();
    m_judgeSprite->setScale( 0.4f );
    m_judgeSprite->setOpacity( 255 );

    std::stringstream sstr;

    switch ( p_judge ) {
        case Perfect:
            sstr << "perfect: " << ++m_perfectCount;
            m_perfectLabel->setString( sstr.str() );
            m_judgeSprite->setSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "perfect.png" ) );
            break;
        case Excellent:
            sstr << "good: " << ++m_excellentCount;
            m_excellentLabel->setString( sstr.str() );
            m_judgeSprite->setSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "good.png" ) );
            break;
        case Leak:
            sstr << "miss: " << ++m_leakCount;
            m_leakLabel->setString( sstr.str() );
            m_judgeSprite->setSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "miss.png" ) );
            break;
    }

    m_judgeSprite->runAction( Sequence::create( EaseBackOut::create( ScaleTo::create( 0.2f, adaptation() ) ), ActionFloat::create( 0.2f, 0.0f, 1.0f, nullptr), EaseIn::create( ActionFloat::create( 0.1f, 0.0f, 1.0f,                                                                                                                                                                            [this]( const float t_val ){
        m_judgeSprite->setOpacity( 255 - (int)( t_val * 255.0f ) );
    } ), 1.0f) , NULL) );
}


CadenceGameScene::~CadenceGameScene( void )
{
    if( AudioEngine::getState( m_backgroundMusicHandle ) == AudioEngine::AudioState::PLAYING )
    {
        AudioEngine::stop( m_backgroundMusicHandle );
    }
}
