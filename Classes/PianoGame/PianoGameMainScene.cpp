//
//  PianoGameMainScene.cpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/10/24.
//

#include "PianoGameMainScene.h"


#include "MainScene.h"
#include "SimpleAudioEngine.h"
#include "Common.h"

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "PianoGame.hpp"

USING_NS_CC;
using namespace  rapidjson;
using namespace cocos2d::ui;

#define COUNTDOWN 3.0f
#define MOVE_TIME 0.5f
#define FAULT_TOLERANT_TIME 0.1f

Scene * PianoGameMainScene::CreateScene()
{
    return create();
}

std::string PianoGameMainScene::s_backgroundMusic = "PianoGame/MusicScore/DaeJangGeum/bgm.wav";
std::string PianoGameMainScene::s_gameJson = "PianoGame/MusicScore/DaeJangGeum/musicScore.json";

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
    
    m_perfectCount = 0;
    m_excellentCount = 0;
    m_leakCount = 0;
    m_musicListShowState = false;
    
    TexturePacker::PianoGame::addSpriteFramesToCache();
    
    auto t_visibleSizeHalf = Director::getInstance()->getVisibleSize() * 0.5f;
    Vec2 t_origin = Director::getInstance()->getVisibleOrigin();
    
    //layer
    
    m_musicListLayer = PianoGameMusicListLayer::create();
    m_musicListLayer->setPosition( Vec2( t_origin.x + t_visibleSizeHalf.width, t_origin.y + t_visibleSizeHalf.height ) );
    addChild( m_musicListLayer, 1001 );
    
    ///-----------
    loadMusic();
    
    //test
    
    m_perfectLabel = Label::createWithSystemFont( "perfect: ", "Marker Felt", 20 );
    m_perfectLabel->setColor( Color3B( 255, 0, 0 ) );
    
    m_perfectLabel->setPosition( Vec2( t_origin.x + 30.0f, t_origin.y + t_visibleSizeHalf.height * 2.0f - 50.0f ) );
    m_perfectLabel->setAnchorPoint( Vec2( 0.0f, 0.5f ) );
    
    addChild( m_perfectLabel, 1000 );
    
    
    m_excellentLabel = Label::createWithSystemFont( "excellent: ", "Marker Felt", 20 );
    m_excellentLabel->setColor( Color3B( 255, 0, 0 ) );
    
    m_excellentLabel->setPosition( Vec2( t_origin.x + 30.0f, t_origin.y + t_visibleSizeHalf.height * 2.0f - 70.0f ) );
    m_excellentLabel->setAnchorPoint( Vec2( 0.0f, 0.5f ) );
    
    addChild( m_excellentLabel, 1000 );
    
    
    m_leakLabel = Label::createWithSystemFont( "leak: ", "Marker Felt", 20 );
    m_leakLabel->setColor( Color3B( 255, 0, 0 ) );
    
    m_leakLabel->setPosition( Vec2( t_origin.x + 30.0f, t_origin.y + t_visibleSizeHalf.height * 2.0f - 90.0f ) );
    m_leakLabel->setAnchorPoint( Vec2( 0.0f, 0.5f ) );
    
    addChild( m_leakLabel, 1000 );
    
    
    //touch effect
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile( "PianoGame/PianoGameToneTouchedUp.plist" );
    
    m_toneTouchedUpAnimation = Animation::create();
    for( int i = 0; i < 15; ++i )
    {
        std::stringstream sstr;
        sstr << "PianoGameToneTouchedUp_" << i << ".png";
        m_toneTouchedUpAnimation->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( sstr.str() ) );
    }
    
    m_toneTouchedUpAnimation->setDelayPerUnit(0.02f);
    m_toneTouchedUpAnimation->setLoops( 1 );
    m_toneTouchedUpAnimation->retain();
    m_toneTouchedUpAnimation->setRestoreOriginalFrame(true);
    
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile( "PianoGame/PianoGameToneTouchedDown.plist" );
    m_toneTouchedDownAnimation = Animation::create();
    
    for( int i = 0; i < 15; ++i )
    {
        std::stringstream sstr;
        sstr << "PianoGameToneTouchedDown_" << i << ".png";
        m_toneTouchedDownAnimation->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( sstr.str() ) );
    }
    
    m_toneTouchedDownAnimation->setDelayPerUnit(0.02f);
    m_toneTouchedDownAnimation->setLoops( 1 );
    m_toneTouchedDownAnimation->retain();
    m_toneTouchedDownAnimation->setRestoreOriginalFrame(true);
    
    //background effect
//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile( "PianoGame/PianoGameBGEffect_1.plist" );
//    auto t_backgroundEffectAnimation_1 = Animation::create();
//    for( int i = 0; i < 10; ++i )
//    {
//        std::stringstream sstr;
//        sstr << "PianoGameBGEffect_1_" << i << ".png";
//        t_backgroundEffectAnimation_1->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( sstr.str() ) );
//    }
//
//    t_backgroundEffectAnimation_1->setDelayPerUnit(0.02f);
//    t_backgroundEffectAnimation_1->setLoops( -1 );
//    t_backgroundEffectAnimation_1->retain();
    
    
    auto t_centerPoit = Vec2( t_origin.x + t_visibleSizeHalf.width, t_origin.y + t_visibleSizeHalf.height );
    
    auto t_background = Sprite::create( "PianoGameBackground.png" );
    auto t_backgroundSizeHalf = t_background->getContentSize() * 0.5f;
    t_background->setScale( t_visibleSizeHalf.width / t_backgroundSizeHalf.width , t_visibleSizeHalf.height / t_backgroundSizeHalf.height );
    t_background->setPosition( t_centerPoit );
    
    addChild( t_background );
    
    m_leftRound = Sprite::create( "PianoGame/leftRound.png" );
    m_leftRound->setScale( adaptation() * 0.62f );
    m_leftRound->setPosition( Vec2( t_origin.x + t_visibleSizeHalf.width * 0.6f, t_origin.y + t_visibleSizeHalf.height ) );
    addChild( m_leftRound );
    
    m_rightRound = Sprite::create( "PianoGame/rightRound.png" );
    m_rightRound->setScale( adaptation() * 0.62f );
    m_rightRound->setPosition( Vec2( t_origin.x + t_visibleSizeHalf.width * 2.3f, t_origin.y + t_visibleSizeHalf.height ) );
    addChild( m_rightRound );
    
//    auto t_backgroundEffectAnimation_1_player = Sprite::create();
//    t_backgroundEffectAnimation_1_player->setPosition( Vec2( t_origin.x + t_visibleSizeHalf.width * 2.0f - 30.0f, t_origin.y + t_visibleSizeHalf.height ) );
//    addChild( t_backgroundEffectAnimation_1_player );
//
//    auto t_backgroundEffectAnimation_1_animate = Animate::create( t_backgroundEffectAnimation_1 );
//    t_backgroundEffectAnimation_1_player->runAction( t_backgroundEffectAnimation_1_animate );
    
    
    //menu home
    auto t_home = Button::create( TexturePacker::PianoGame::home, TexturePacker::PianoGame::home, "", Widget::TextureResType::PLIST  );
    auto t_homeSizeHalf = t_home->getContentSize() * 0.5f * adaptation();
    t_home->setScale( adaptation() );
    t_home->setPosition( Vec2( t_origin.x + t_homeSizeHalf.width + 15.0f, t_origin.y + t_visibleSizeHalf.height * 2.0f - t_homeSizeHalf.height - 10.0f ) );
    t_home->setTag( 1 );
    this->addChild( t_home );
    
    m_buttonList.push_back( t_home );
    
    touchAnswer( t_home, [this]( Ref * p_ref ){
        buttonClick( ((Button*)p_ref)->getTag() );
    }, adaptation() * 1.2f, adaptation() );
    
    //menu playState
    
    m_playState = Button::create( TexturePacker::PianoGame::playStatePause, TexturePacker::PianoGame::playStatePause, "", Widget::TextureResType::PLIST  );
    m_playState->setScale( adaptation() );
    
    auto t_playStateSizeHalf = m_playState->getContentSize() * 0.5f * m_playState->getScale();
    m_playState->setPosition( Vec2( t_origin.x + t_visibleSizeHalf.width * 2.0f - t_playStateSizeHalf.width * 3.0f - 30.0f, t_origin.y + t_visibleSizeHalf.height * 2.0f - t_playStateSizeHalf.height - 10.0f ) );
    m_playState->setTag( 2 );
    
    this->addChild( m_playState );
    
    m_buttonList.push_back( m_playState );
    
    touchAnswer( m_playState, [this]( Ref * p_ref ){
        Button * t_button = (Button*)p_ref;
        buttonClick( t_button->getTag() );
        
        changePlayButtonState();
        
    }, adaptation() * 1.2f, adaptation() );
    
    //menu musicList
    auto t_musicList = Button::create( TexturePacker::PianoGame::musicList, TexturePacker::PianoGame::musicList, "", Widget::TextureResType::PLIST  );
    t_musicList->setScale( adaptation() );
    
    auto t_musicListSizeHalf = t_musicList->getContentSize() * 0.5f * t_musicList->getScale();
    t_musicList->setPosition( Vec2( t_origin.x + t_visibleSizeHalf.width * 2.0f - t_musicListSizeHalf.width - 20.0f, t_origin.y + t_visibleSizeHalf.height * 2.0f - t_musicListSizeHalf.height - 10.0f ) );
    t_musicList->setTag( 3 );
    
    this->addChild( t_musicList );
    
    m_buttonList.push_back( t_musicList );
    
    touchAnswer( t_musicList, [this]( Ref * p_ref ){
        buttonClick( ((Button*)p_ref)->getTag() );
    }, adaptation() * 1.2f, adaptation() );
    
    
    
    //background
    auto t_BGRound = Sprite::create("PianoGameBGRound.png");
    auto t_BGRoundSizeHalf = t_BGRound->getContentSize() * 0.5f;
    
    auto t_BGRoundScale = t_visibleSizeHalf.height / ( t_BGRoundSizeHalf.height + 10.0f );
    
    t_BGRound->setScale( t_BGRoundScale, t_BGRoundScale );
    t_BGRound->setPosition( t_centerPoit );
    
    addChild( t_BGRound );
    
    auto t_centerControl = TexturePacker::PianoGame::createCenterControlSprite();
    auto t_centerControlSizeHalf = t_centerControl->getContentSize() * 0.5f;
    m_centerControlPosition = t_centerPoit + Vec2( -3.0f, 3.0f );
    
    m_centerControlScale = t_visibleSizeHalf.height / ( t_centerControlSizeHalf.height * 1.2f );
    
    t_centerControl->setScale( m_centerControlScale );
    t_centerControl->setPosition( m_centerControlPosition );
    
    addChild( t_centerControl );
    
    m_keyBottom = t_centerControlSizeHalf.width * m_centerControlScale * 0.4546f * 0.5f;
    m_drumR = (t_centerControlSizeHalf.height * m_centerControlScale) - m_keyBottom;
    
    auto t_minus = TexturePacker::PianoGame::createMinusSprite();
    auto t_minusSizeHalf = t_minus->getContentSize() * 0.5f * m_centerControlScale;
    t_minus->setScale( m_centerControlScale );
    t_minus->setPosition( m_centerControlPosition + Vec2( cos( PI / 180.0f * 90.0f ) * m_drumR - t_minusSizeHalf.width - 0.5f, sin( PI / 180.0f * 90.0f) * m_drumR ) );
    addChild( t_minus );
    
    auto t_plus = TexturePacker::PianoGame::createPlusSprite();
    auto t_plusSizeHalf = t_plus->getContentSize() * 0.5f * m_centerControlScale;
    t_plus->setScale( m_centerControlScale );
    t_plus->setPosition( m_centerControlPosition + Vec2( cos( PI / 180.0f * 90.0f ) * m_drumR + t_plusSizeHalf.width + 0.5f , sin( PI / 180.0f * 90.0f) * m_drumR ) );
    addChild( t_plus );
    
    m_startPos = Vec2( cos( PI / 180.0f * 150.0f) * m_drumR, sin( PI / 180.0f * 150.0f ) * m_drumR );
    
    m_angleOffset = 165.0f;
    
    //keys
    for( int i = 0; i < 8; ++i ){
        
        std::stringstream sstr;
        
        sstr << "key_" << i << ".png";
        
        float t_angele = PI / 180.0f * ( m_angleOffset + (float)i * 30.0f );
        
        
        auto t_spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName( sstr.str() );
        
        auto t_key = Sprite::createWithSpriteFrame( t_spriteFrame );
        t_key->setScale( m_centerControlScale );
        auto t_keyOffset =  Vec2( cos( t_angele ) * m_drumR, sin( t_angele ) * m_drumR );
        auto t_keyPos = m_centerControlPosition + t_keyOffset;
        t_key->setPosition( t_keyPos );
        
        m_sequeueDirection.push_back( t_keyOffset );
        
        auto t_shadow = TexturePacker::PianoGame::createShadowSprite();
        t_shadow->setScale( m_centerControlScale );
        t_shadow->setPosition( t_keyPos + Vec2( 5.0f, -6.0f ) );
        
        auto t_animateBoxUp = Sprite::create();
        t_animateBoxUp->setScale( m_centerControlScale );
        t_animateBoxUp->setPosition( t_keyPos );
        
        auto t_animateBoxDown = Sprite::create();
        t_animateBoxDown->setScale( m_centerControlScale );
        t_animateBoxDown->setPosition( t_keyPos );
        
        auto t_keyMark = TexturePacker::PianoGame::createMarkSprite();
        t_keyMark->setScale( m_centerControlScale );
        t_keyMark->setPosition( t_keyPos );
        
        t_keyMark->setVisible( false );
        
        
        addChild( t_shadow, 1 );
        addChild( t_key, 5 );
        addChild( t_animateBoxUp, 6 );
        addChild( t_animateBoxDown, 2 );
        addChild( t_keyMark, 10 );
        
        m_toneUpAnimateSpriteList.push_back( t_animateBoxUp );
        m_toneDownAnimateSpriteList.push_back( t_animateBoxDown );
        m_toneSpriteList.push_back( t_key );
        m_toneMarkSpriteList.push_back( t_keyMark );
    }
    
    m_judgeSprite = TexturePacker::PianoGame::createPerfectSprite();
    m_judgeSprite->setPosition( m_centerControlPosition + Vec2( cos( PI * 0.5f ) * (m_drumR - m_keyBottom) * 0.7f, sin( PI * 0.5f ) * (m_drumR - m_keyBottom) * 0.7f ) );
    m_judgeSprite->setScale( adaptation() );
    m_judgeSprite->setOpacity( 0 );
    addChild( m_judgeSprite, 100 );
    
    m_countDownSprite = Sprite::create();
    m_countDownSprite->setPosition( m_centerControlPosition );
    m_countDownSprite->setScale( adaptation() );
    addChild( m_countDownSprite, 100 );
    
    
    auto t_touchListener = EventListenerTouchAllAtOnce::create();
    t_touchListener->setEnabled( true );
    
    t_touchListener->onTouchesBegan = [this]( const std::vector<Touch*>& touches, Event *event ){
        
        for( auto t_touche : touches ){
            auto t_location = t_touche->getLocation();
            auto t_touchToneIndex = contains( t_location );
            
            m_touchIndex[t_touche->getID()] = t_touchToneIndex;
            if( t_touchToneIndex <= -1 )
            {
                continue;
            }

            toneTouched( t_touchToneIndex );
        }
    };
    
    t_touchListener->onTouchesEnded = [this]( const std::vector<Touch*>& touches, Event *event ){
        for( auto t_touche : touches ){
            if( m_touchIndex[t_touche->getID()] > -1 ){
                toneTouchEnded( m_touchIndex[t_touche->getID()] );
                m_touchIndex[t_touche->getID()] = -1;
            }
        }
    };
    
    t_touchListener->onTouchesMoved = [this]( const std::vector<Touch*>& touches, Event *event ){
        for( auto t_touche : touches ){
            
            auto t_location = t_touche->getLocation();
            auto t_touchToneIndex = contains( t_location );
            
            int t_prveTouchIndex = m_touchIndex[t_touche->getID()];
            
            m_touchIndex[t_touche->getID()] = t_touchToneIndex;
            
            if( t_prveTouchIndex != t_touchToneIndex && t_prveTouchIndex > -1 )
            {
                toneTouchEnded( t_prveTouchIndex );
            }
            
            if( t_touchToneIndex <= -1 || t_touchToneIndex == t_prveTouchIndex )
            {
                continue;
            }
            
            toneTouched( t_touchToneIndex );
        }
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
    
    m_realTime += p_delta;
    
    m_leftRound->setRotation( fmod( m_realTime * 360.0f * 1.2f , 360.0f )  );
    m_rightRound->setRotation( fmod( m_realTime * 360.0f * 1.2f , 360.0f )  );
    
    bool t_updateMusicTime = true;
    if( !m_playing )
    {
        return;
    }
    
    if( m_currPlayTime < COUNTDOWN )
    {
        t_updateMusicTime = false;
        m_currPlayTime += p_delta;
        
        
        int t_countDown = (int)floor( COUNTDOWN - m_currPlayTime - 0.2f ) + 1;
        
        //倒计时时间
        if( t_countDown < 4 && t_countDown != m_countDownNumber )
        {
            m_countDownNumber = t_countDown;
            m_countDownSprite->setVisible( true );
            
            std::stringstream t_sstr;
            if( m_countDownNumber > 0 ){
                t_sstr << "countDown_" << m_countDownNumber << ".png";
                
                auto t_spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName( t_sstr.str() );
                m_countDownSprite->setSpriteFrame( t_spriteFrame );
                m_countDownSprite->setScale( 2.0f * adaptation() );
                m_countDownSprite->setOpacity( 0 );
                
                m_countDownSprite->runAction( EaseOut::create( ActionFloat::create(0.2f, 0.0, 1.0, [this]( const float t_val ){
                    m_countDownSprite->setScale( (2.0f - t_val) * adaptation() );
                    m_countDownSprite->setOpacity( (int)(255.0f * t_val) );
                }), 1.0f) );
            }else{
                m_countDownSprite->setVisible( false );
            }
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
            
            auto t_findRes = std::find( m_sequeueToneNoOctave.begin(), m_sequeueToneNoOctave.end(), t_strTone );
            if( t_findRes == m_sequeueToneNoOctave.end() )
            {
                continue;
            }
            int t_index = (int)( t_findRes - m_sequeueToneNoOctave.begin() );
            std::stringstream t_sstr;
            t_sstr << "key_" << t_index << ".png";
            
            auto t_spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName( t_sstr.str() );
            
            auto t_sprite = Sprite::createWithSpriteFrame( t_spriteFrame );
            t_sprite->setScale( m_centerControlScale );
            addChild( t_sprite );
            
            GameTone t_gameTone{
                m_musicScore[m_showBollIndex].time,
                t_index,
                item,
                t_sprite
            };
            m_gameTones.push_back( t_gameTone );
        }
        
        m_showBollIndex++;
    }
    
    for( int i = 0; i < m_gameTones.size(); ++i )
    {
        auto item = m_gameTones[i];
        auto t_timeOffset = m_musicTime - item.time + MOVE_TIME;
        
////        自动播放声音
//        if( abs( m_musicTime - item.time ) <= 0.01f )
//        {
//            std::stringstream t_sstr;
//            t_sstr << "PianoGame/Piano/" << item.tone << ".wav";
//            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect( t_sstr.str().c_str() );
//        }
        
        if( m_musicTime - item.time > FAULT_TOLERANT_TIME )
        {
            
            //miss
            auto t_mark = m_toneMarkSpriteList[item.index];
            t_mark->stopAllActions();
            t_mark->setVisible(true);
            t_mark->setScale( 0.1f );
            t_mark->setOpacity( 255 );
            
            t_mark->runAction( Sequence::create( ActionFloat::create( 0.3f, 0.0f, 1.0f, [=]( const float p_val ){
                t_mark->setScale(  p_val * m_centerControlScale );
                
            }), ActionFloat::create( 0.1f, 0.0f, 1.0f, [=]( const float p_val ){
                t_mark->setOpacity( 255 - (int)( p_val * 255.0f ) );
            }) , NULL) );
            
            changeJudge( Leak );
            
            removeChild( item.ball );
            m_gameTones.erase( m_gameTones.begin() + i );
            --i;
            continue;
        }
        item.ball->setPosition( m_centerControlPosition + t_timeOffset / MOVE_TIME * m_sequeueDirection[item.index] );
    }
    
    if( m_showBollIndex >= m_musicScore.size() && m_musicTime > m_musicScore[m_showBollIndex].time && m_gameTones.size() <= 0 )
    {
        m_musicTime = 0.0f;
        m_currPlayTime = 0.0f;
        m_playing = false;
        m_countDownNumber = 0;
        m_backgroundMusicPlaying = false;
        m_showBollIndex = 0;
        changePlayButtonState();
    }
}

int PianoGameMainScene::contains( const cocos2d::Vec2 & p_point )
{
    int t_touchIndex = -1;
    auto t_distance = p_point.getDistance( m_centerControlPosition );
    
    if( t_distance <= m_drumR + m_keyBottom && t_distance >= m_drumR - m_keyBottom )
    {
        auto t_point = p_point - m_centerControlPosition;
        
        auto t_angle = m_startPos.getAngle( t_point ) / PI * 180.0f;
        if( t_angle >= 0.0f )
        {
            t_touchIndex = (int)floor(t_angle / 30.0f);
        }else if( t_angle <= -120.0f ){
            t_touchIndex = 7 - (int)floor( abs( ( (t_angle + 120.0f) / 30.0f ) ) );
        }
    }
    
    return t_touchIndex;
}

void PianoGameMainScene::toneTouched( const int p_index )
{

    std::stringstream sstr;
    sstr << "keyTouch_" << p_index << ".png";
    
    m_toneSpriteList[p_index]->setSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( sstr.str() ) );
    
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
        Animate * t_animateUp = Animate::create( m_toneTouchedUpAnimation );
        int t_index = m_gameTones[t_toneIndex].index;
        m_toneUpAnimateSpriteList[ t_index ]->stopAllActions();
        m_toneUpAnimateSpriteList[ t_index ]->setSpriteFrame(  SpriteFrameCache::getInstance()->getSpriteFrameByName( "PianoGameToneTouchedUp_0.png" ) );
        m_toneUpAnimateSpriteList[ t_index ]->runAction( t_animateUp );
        
        Animate * t_animateDown = Animate::create( m_toneTouchedDownAnimation );
        m_toneDownAnimateSpriteList[ t_index ]->stopAllActions();
        m_toneDownAnimateSpriteList[ t_index ]->setSpriteFrame(  SpriteFrameCache::getInstance()->getSpriteFrameByName( "PianoGameToneTouchedDown_0.png" ) );
        m_toneDownAnimateSpriteList[ t_index ]->runAction( t_animateDown );
        
        removeChild( m_gameTones[t_toneIndex].ball );
        m_gameTones.erase( m_gameTones.begin() + t_toneIndex );
        
        if( t_time <= 0.05f )
        {
            changeJudge( Perfect );
        }else{
            changeJudge( Excellent );
        }
        
        std::stringstream t_sstr;
        t_sstr << "PianoGame/Piano/" << m_gameTones[t_toneIndex].tone << ".wav";
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect( t_sstr.str().c_str() );
        
    }else{
        //miss
        
        std::stringstream t_sstr;
        t_sstr << "PianoGame/Piano/" << m_sequeueTone[p_index] << ".wav";
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect( t_sstr.str().c_str() );
        
        changeJudge( Leak );
    }
}

void PianoGameMainScene::toneTouchEnded( const int p_index )
{
    std::stringstream sstr;
    sstr << "key_" << p_index << ".png";
    
    m_toneSpriteList[p_index]->setSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( sstr.str() ) );
}

 void PianoGameMainScene::changePlayButtonState()
{
    if( m_playing )
    {
        m_playState->loadTextureNormal( TexturePacker::PianoGame::playStatePlay, Widget::TextureResType::PLIST  );
        m_playState->loadTexturePressed( TexturePacker::PianoGame::playStatePlay, Widget::TextureResType::PLIST  );
    }else{
        m_playState->loadTextureNormal( TexturePacker::PianoGame::playStatePause, Widget::TextureResType::PLIST  );
        m_playState->loadTexturePressed( TexturePacker::PianoGame::playStatePause, Widget::TextureResType::PLIST  );
    }
}

void PianoGameMainScene::changeJudge( const Judge p_judge )
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
            sstr << "excellent: " << ++m_excellentCount;
            m_excellentLabel->setString( sstr.str() );
            m_judgeSprite->setSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "excellent.png" ) );
            break;
        case Leak:
            sstr << "leak: " << ++m_leakCount;
            m_leakLabel->setString( sstr.str() );
            m_judgeSprite->setSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "leak.png" ) );
            break;
    }
    
    m_judgeSprite->runAction( Sequence::create( EaseBackOut::create( ScaleTo::create( 0.2f, adaptation() ) ), ActionFloat::create( 0.2f, 0.0f, 1.0f, nullptr), EaseIn::create( ActionFloat::create( 0.1f, 0.0f, 1.0f,                                                                                                                                                                            [this]( const float t_val ){
        m_judgeSprite->setOpacity( 255 - (int)( t_val * 255.0f ) );
    } ), 1.0f) , NULL) );
}

void PianoGameMainScene::buttonClick( const int p_tag )
{
    switch( p_tag )
    {
        case 1:
            Director::getInstance()->replaceScene( MainScene::CreateScene() );
            break;
        case 2:
            if( !m_playing ){
                m_playing = true;
                m_currPlayTime = 0.0f;
                m_countDownSprite->setVisible( true );
                
                if( m_showBollIndex <= 0 )
                {
                    m_perfectCount = 0;
                    m_perfectLabel->setString( "perfect: 0" );
                    
                    m_excellentCount = 0;
                    m_excellentLabel->setString( "excellent: 0" );
                    
                    m_leakCount = 0;
                    m_leakLabel->setString( "leak: 0" );
                }
                
            }else{
                m_playing = false;
                m_countDownSprite->setVisible( false );
                CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
                CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
            }
            break;
        case 3:
            if( m_musicListShowState )
            {
                m_musicListLayer->hide();
                loadMusic();
            }else{
                m_musicListLayer->show();
            }
            m_musicListShowState = !m_musicListShowState;
            break;
    }
}


void PianoGameMainScene::loadMusic( void )
{
    
    m_musicTime = 0.0f;
    m_currPlayTime = 0.0f;
    
    m_perfectCount = 0;
    m_excellentCount = 0;
    m_leakCount = 0;
    
    m_showBollIndex = 0;
    
    
    auto t_music = m_musicListLayer->getSelectMusic();
    
    s_backgroundMusic = t_music.first;
    s_gameJson = t_music.second;
    
    m_backgroundMusicPlaying = false;
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    
    
    m_musicScore.clear();
    m_sequeueTone.clear();
    m_sequeueToneNoOctave.clear();
    Document t_readdoc;
    
    //load json
    std::string t_fileStr = FileUtils::getInstance()->getStringFromFile( s_gameJson );
    
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
        
        std::string t_tone = t_keys[i].GetString();
        m_sequeueTone.push_back( t_tone );
        
        
        auto t_toneNoOctave = split( t_tone, "+" )[0];
        m_sequeueToneNoOctave.push_back( t_toneNoOctave );
        
        std::stringstream t_audio;
        t_audio << "PianoGame/Piano/" << t_toneNoOctave << ".wav";
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect( t_audio.str().c_str() );
        
        t_audio.str("");
        t_audio << "PianoGame/Piano/" << t_toneNoOctave << "+1.wav";
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect( t_audio.str().c_str() );
        
        t_audio.str("");
        t_audio << "PianoGame/Piano/" << t_toneNoOctave << "+2.wav";
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect( t_audio.str().c_str() );
        
    }
    
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

PianoGameMainScene::~PianoGameMainScene( void )
{
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
    
    TexturePacker::PianoGame::removeSpriteFramesFromCache();
    
    m_toneTouchedUpAnimation->release();
}

