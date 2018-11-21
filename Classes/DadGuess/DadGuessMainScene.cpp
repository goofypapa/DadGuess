//
//  DadGuessMainScene.cpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/11/20.
//

#include "DadGuessMainScene.h"
#include "DadGuessMain.hpp"
#include "Common.h"
#include "MainScene.h"
#include "DadGuessUpdateScene.h"

#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

bool DadGuessMainScene::init( void )
{
    if( !BaseScene::init() )
    {
        return false;
    }
    
    auto t_visibleSizeHalf = Director::getInstance()->getVisibleSize() * 0.5f;
    Vec2 t_origin = Director::getInstance()->getVisibleOrigin();
    
    auto t_background = TexturePacker::DadGuessMain::createCaicai_home_bjSprite();
    
    t_background->setPosition( Vec2( t_visibleSizeHalf.width + t_origin.x, t_visibleSizeHalf.height + t_origin.y ) );
    auto t_backgroundSizeHalf = t_background->getContentSize() * 0.5f;
    
    t_background->setScale( t_visibleSizeHalf.width / t_backgroundSizeHalf.width, t_visibleSizeHalf.height / t_backgroundSizeHalf.height );
    
    addChild( t_background, 0 );
    
    auto t_homeButton = Button::create( TexturePacker::DadGuessMain::caicai_home_icon_gohome, TexturePacker::DadGuessMain::caicai_home_icon_gohome, "", Button::TextureResType::PLIST );
    t_homeButton->setScale( adaptation() );
    auto t_homeButtonSizeHalf = t_homeButton->getContentSize() * t_homeButton->getScale() * 0.5f;
    
    t_homeButton->setPosition( Vec2( t_origin.x + t_homeButtonSizeHalf.width + 15.0f, t_origin.y + t_visibleSizeHalf.height * 2.0f - t_homeButtonSizeHalf.height - 15.0f ) );
    
    addChild( t_homeButton );
    
    touchAnswer( t_homeButton, []( Ref * p_target ){
        Director::getInstance()->replaceScene( MainScene::create() );
        DadGuessUpdateScene::unCacheResource();
    }, adaptation() * 1.2f, adaptation());
    
    
    
    return true;
}
