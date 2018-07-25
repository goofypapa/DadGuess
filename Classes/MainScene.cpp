//
//  MainScene.cpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/7/24.
//

#include "MainScene.h"
#include "Common.h"
#include "Recources/Main.hpp"
#include "ui/CocosGUI.h"

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
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    float t_PaddingTop = ( visibleSize.height - origin.y ) * 0.15f;
    
    auto t_backgroud = Sprite::create( "BackgroundImage.png" );
    if (t_backgroud != nullptr)
    {
        // position the sprite on the center of the screen
        t_backgroud->setPosition( Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
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
        this->addChild( t_CloundLeft, 2 );
    }
    
    auto t_CloundRight = Sprite::create("Cloud.png");
    if( t_CloundRight != nullptr )
    {
        t_CloundRight->setScale( adaptation() );
        t_CloundRight->setPosition( Vec2(visibleSize.width / 4.0f * 3.0f + origin.x, visibleSize.height + origin.y - t_CloundLeftSize.height * 0.5f - 15.0f - t_PaddingTop) );
        this->addChild( t_CloundRight, 2 );
    }
    
    auto t_Settings = MenuItemSprite::create( TexturePacker::Main::createMainSettingSprite() , TexturePacker::Main::createMainSettingSprite() , CC_CALLBACK_1( MainScene::settingsCallBack, this ) );
    t_Settings->setScale( adaptation() );
    auto t_SettingsSize = t_Settings->getContentSize();
    t_Settings->setPosition( Vec2( visibleSize.width - t_SettingsSize.width * 2.5f, visibleSize.height + origin.y - t_SettingsSize.height * 0.5f ) );
    
    {
        auto t_menu = Menu::create( t_Settings, NULL );
        t_menu->setPosition( Vec2::ZERO );
        this->addChild( t_menu, 1 );
    }
    

    return true;
}

MainScene::~MainScene()
{
    TexturePacker::Main::removeSpriteFramesFromCache();
}

void MainScene::settingsCallBack( cocos2d::Ref* pSender )
{
    
}
