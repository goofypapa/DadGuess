//
//  MainScene.cpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/7/24.
//

#include "MainScene.h"
#include "Common.h"
#include "Main.hpp"
#include "Dialog.hpp"

#include "WebViewScene.h"


USING_NS_CC;
using namespace cocos2d::ui;


Scene * MainScene:: CreateScene( void )
{
    return create();
}

bool MainScene::init( void )
{
    if( !Scene::init() )
    {
        return false;
    }
    
    //加载贴图
    TexturePacker::Main::addSpriteFramesToCache();
    TexturePacker::Dialog::addSpriteFramesToCache();
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    float t_PaddingTop = ( visibleSize.height - origin.y ) * 0.15f;
    
    auto t_backgroud = Sprite::create( "BackgroundImage.png" );
    if (t_backgroud != nullptr)
    {
        // position the sprite on the center of the screen
        t_backgroud->setPosition( Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y) );
        auto t_backgroundSize = t_backgroud->getContentSize();
        
        t_backgroud->setScale( visibleSize.width / t_backgroundSize.width, visibleSize.height / t_backgroundSize.height );
        
        this->addChild( t_backgroud, 0 );
    }
    
    auto t_CloundLeft = Sprite::create("Cloud.png");
    auto t_CloundLeftSize = t_CloundLeft->getContentSize();
    if( t_CloundLeft != nullptr )
    {
        t_CloundLeft->setScale( adaptation() );
        t_CloundLeft->setPosition( Vec2(visibleSize.width / 4.0f + origin.x, visibleSize.height + origin.y - t_CloundLeftSize.height * 0.5f - 5.0f - t_PaddingTop ) );
        this->addChild( t_CloundLeft, 0 );
    }
    
    auto t_CloundRight = Sprite::create("Cloud.png");
    if( t_CloundRight != nullptr )
    {
        t_CloundRight->setScale( adaptation() );
        t_CloundRight->setPosition( Vec2(visibleSize.width / 4.0f * 3.0f + origin.x, visibleSize.height + origin.y - t_CloundLeftSize.height * 0.5f - 15.0f - t_PaddingTop) );
        this->addChild( t_CloundRight, 0 );
    }
    
    
    //用户头像
    auto t_personalHead = Button::create( TexturePacker::Dialog::per_touxiang, TexturePacker::Dialog::per_touxiang, "", Widget::TextureResType::PLIST  );
    t_personalHead->setScale( adaptation() );
    auto t_personalHeadSizeHelf = t_personalHead->getContentSize() * adaptation() * 0.5f;
    t_personalHead->setPosition( Vec2( origin.x + t_personalHeadSizeHelf.width + 15.0f , origin.y + visibleSize.height - t_personalHeadSizeHelf.height - 10.0f ) );
    this->addChild( t_personalHead );
    m_mainSceneButtons.push_back( t_personalHead );
    
    touchAnswer( t_personalHead , [this]( Ref * p_ref ){
        m_disenableAllButton();
        personalHeadOnClick();
    }, adaptation() * 1.1f, adaptation() );
    
    
    auto t_Settings = Button::create( TexturePacker::Main::mainSetting, TexturePacker::Main::mainSetting, "", Widget::TextureResType::PLIST  );
    t_Settings->setScale( adaptation() );
    auto t_SettingsSize = t_Settings->getContentSize();
    t_Settings->setPosition( Vec2( visibleSize.width - t_SettingsSize.width * 2.5f * adaptation() - 45.0f, visibleSize.height + origin.y - t_SettingsSize.height * 0.5f * adaptation() - 20.0f ) );
    this->addChild( t_Settings );
    m_mainSceneButtons.push_back( t_Settings );

    touchAnswer( t_Settings, [this]( Ref * p_ref ){
        m_disenableAllButton();
        settingsOnClick();
    }, adaptation() * 1.1f, adaptation() );

    auto t_Message = Button::create( TexturePacker::Main::mainMessage, TexturePacker::Main::mainMessage, "", Widget::TextureResType::PLIST  );
    t_Message->setScale( adaptation() );
    auto t_MessageSize = t_Message->getContentSize();
    t_Message->setPosition( Vec2( visibleSize.width - t_MessageSize.width * 1.5f * adaptation() - 30.0f, visibleSize.height + origin.y - t_MessageSize.height * 0.5f * adaptation() - 20.0f ) );
    this->addChild( t_Message );
    m_mainSceneButtons.push_back( t_Message );

    touchAnswer( t_Message, [this]( Ref * p_ref ){
        dadpatCallBack( this );
    }, adaptation() * 1.1f, adaptation() );

    auto t_NewMessage = TexturePacker::Main::createMainNewMessageSprite();
    auto t_NewMessageSize = t_NewMessage->getContentSize();
    t_NewMessage->setPosition( Vec2( t_MessageSize.width - t_NewMessageSize.width * 0.8f, t_MessageSize.height - t_NewMessageSize.height * 0.6f ) );
    t_Message->addChild( t_NewMessage );

    auto t_Notice = Button::create( TexturePacker::Main::mainNotice, TexturePacker::Main::mainNotice, "", Widget::TextureResType::PLIST  );
    t_Notice->setScale( adaptation() );
    auto t_NoticeSize = t_Notice->getContentSize();
    t_Notice->setPosition( Vec2( visibleSize.width - t_NoticeSize.width * 0.5f * adaptation() - 15.0f, visibleSize.height + origin.y - t_NoticeSize.height * 0.5f * adaptation() - 20.0f ) );
    this->addChild( t_Notice );
    m_mainSceneButtons.push_back( t_Notice );
    touchAnswer( t_Notice, [this]( Ref * p_ref ){
        dadpatCallBack( this );
    }, adaptation() * 1.1f, adaptation() );

    auto t_Baike = Button::create( TexturePacker::Main::mainBaike, TexturePacker::Main::mainBaike, "", Widget::TextureResType::PLIST  );
    t_Baike->setScale( adaptation() );

    auto t_BaikeSize = t_Baike->getContentSize();
    float t_ItemSpace = (visibleSize.width - 30.0f - t_BaikeSize.width * adaptation() * 3 ) / 4.0f;
    float t_HeadHeight = t_NoticeSize.height * adaptation() + 20.0f;
    t_Baike->setPosition( Vec2( origin.x + 15.0f + t_ItemSpace + t_BaikeSize.width * adaptation() * 0.5f, origin.y + ( visibleSize.height - t_HeadHeight ) * 0.5f + 10.0f ) );
    this->addChild( t_Baike );
    m_mainSceneButtons.push_back( t_Baike );
    touchAnswer( t_Baike, [this]( Ref * p_ref ){
        animalCallBack();
    }, adaptation() * 1.1f, adaptation() );
    
    auto t_Game = Button::create( TexturePacker::Main::mainGame, TexturePacker::Main::mainGame, "", Widget::TextureResType::PLIST  );
    t_Game->setScale( adaptation() );

    t_Game->setPosition( Vec2( origin.x + 15.0f + t_ItemSpace * 2.0f + t_BaikeSize.width * adaptation() * 1.5f, origin.y + ( visibleSize.height - t_HeadHeight ) * 0.5f + 10.0f ) );
    this->addChild( t_Game );
    m_mainSceneButtons.push_back( t_Game );
    touchAnswer( t_Game, [this]( Ref * p_ref ){
        dadpatCallBack( this );
    }, adaptation() * 1.1f, adaptation() );

    auto t_Dadpat = Button::create( TexturePacker::Main::mainDadpat, TexturePacker::Main::mainDadpat, "", Widget::TextureResType::PLIST  );
    t_Dadpat->setScale( adaptation() );

    t_Dadpat->setPosition( Vec2( origin.x + 15.0f + t_ItemSpace * 3.0f + t_BaikeSize.width * adaptation() * 2.5f, origin.y + ( visibleSize.height - t_HeadHeight ) * 0.5f + 10.0f ) );
    this->addChild( t_Dadpat );
    m_mainSceneButtons.push_back( t_Dadpat );
    touchAnswer( t_Dadpat, [this]( Ref * p_ref ){
        dadpatCallBack( this );
    }, adaptation() * 1.1f, adaptation() );

    m_enableAllButton = [this](){
        for( Button * button : m_mainSceneButtons )
        {
            button->setTouchEnabled( true );
        }
    };

    m_disenableAllButton = [this](){
        for( Button * button : m_mainSceneButtons )
        {
            button->setTouchEnabled( false );
        }
    };


    //设置框
    m_dialogSettings = DialogSettingsLayer::create();
    m_dialogSettings->setPosition( Vec2( visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y ) );
    this->addChild( m_dialogSettings, 2 );

    m_dialogSettings->hideCallBack = [this](){
        m_enableAllButton();
    };
    
    //用户中心
    m_dialogPersonalCenter = DialogPersonalCenterLayer::create();
    m_dialogPersonalCenter->setPosition( Vec2( visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y ) );
    this->addChild( m_dialogPersonalCenter );
    
    m_dialogPersonalCenter->hideCallBack = [this](){
        m_enableAllButton();
    };

    return true;
}

MainScene::~MainScene()
{
    TexturePacker::Main::removeSpriteFramesFromCache();
    TexturePacker::Dialog::removeSpriteFramesFromCache();
}

void MainScene::personalHeadOnClick( void )
{
    m_dialogPersonalCenter->show();
}

void MainScene::settingsOnClick( void )
{
    m_dialogSettings->show();
}

void MainScene::animalCallBack( void )
{
    Director::getInstance()->replaceScene( WebViewScene::create() );
}

void MainScene::dadpatCallBack( Ref* pSender )
{
    printf( "dadpat click \n" );
}
