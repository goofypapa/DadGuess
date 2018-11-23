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
#include "DadGuessCardListScene.h"

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
    
    auto t_center = Point( t_origin.x + t_visibleSizeHalf.width, t_origin.y + t_visibleSizeHalf.height );
    
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
    
    auto t_logo = TexturePacker::DadGuessMain::createCaicai_home_pic_logoSprite();
    t_logo->setScale( adaptation() );
    auto t_logoSizeHalf = t_logo->getContentSize() * t_logo->getScale() * 0.5f;
    
    t_logo->setPosition( Vec2( t_origin.x + t_visibleSizeHalf.width, t_origin.y + t_visibleSizeHalf.height * 2.0f - t_logoSizeHalf.height - 5.0f ) );
    
    addChild( t_logo );
    
    auto t_product = TexturePacker::DadGuessMain::createCaicai_home_pic_productSprite();
    auto t_productSizeHalf = t_product->getContentSize() * 0.5f;
    
    float t_adaptation = t_visibleSizeHalf.height * 0.35f / t_productSizeHalf.height;
    t_product->setScale( t_adaptation );
    
    t_product->setPosition( Vec2( t_center.x, t_origin.y + t_productSizeHalf.height * t_product->getScale() ) );
    addChild( t_product );
    
    //
    auto t_btnAnimal = Button::create( TexturePacker::DadGuessMain::caicai_home_animal, TexturePacker::DadGuessMain::caicai_home_animal, "", Button::TextureResType::PLIST );
    t_btnAnimal->setScale( t_adaptation );
    auto t_btnAnimalSizeHalf = t_btnAnimal->getContentSize() * t_btnAnimal->getScale() * 0.5f;
    
    t_btnAnimal->setPosition( Vec2( t_center.x - t_btnAnimalSizeHalf.width - 15.0f, t_center.y + t_btnAnimalSizeHalf.height  + 22.0f ) );
    addChild( t_btnAnimal );
    
    //
    auto t_btnAbc = Button::create( TexturePacker::DadGuessMain::caicai_home_abc, TexturePacker::DadGuessMain::caicai_home_abc, "", Button::TextureResType::PLIST );
    t_btnAbc->setScale( t_adaptation );
    auto t_btnAbcSizeHalf = t_btnAbc->getContentSize() * t_btnAbc->getScale() * 0.5f;
    
    t_btnAbc->setPosition( Vec2( t_center.x + t_btnAbcSizeHalf.width - 3.0f, t_center.y  + t_btnAbcSizeHalf.height + 15.0f ) );
    addChild( t_btnAbc );
    
    //
    auto t_btnSky = Button::create( TexturePacker::DadGuessMain::caicai_home_sky, TexturePacker::DadGuessMain::caicai_home_sky, "", Button::TextureResType::PLIST );
    t_btnSky->setScale( t_adaptation  );
    auto t_btnSkySizeHalf = t_btnSky->getContentSize() * t_btnSky->getScale() * 0.5f;
    
    t_btnSky->setPosition( Vec2( t_center.x - t_btnSkySizeHalf.width * 3.0f - 25.0f, t_center.y + 10.0f ) );
    addChild( t_btnSky );
    
    //
    auto t_btnEart = Button::create( TexturePacker::DadGuessMain::caicai_home_earth, TexturePacker::DadGuessMain::caicai_home_earth, "", Button::TextureResType::PLIST );
    t_btnEart->setScale( t_adaptation  );
    auto t_btnEartSizeHalf = t_btnEart->getContentSize() * t_btnEart->getScale() * 0.5f;
    
    t_btnEart->setPosition( Vec2( t_center.x + t_btnEartSizeHalf.width * 3.0f + 3.0f, t_center.y + 10.0f ) );
    addChild( t_btnEart );
    
    //
    auto t_btnHistory = Button::create( TexturePacker::DadGuessMain::caicai_home_history, TexturePacker::DadGuessMain::caicai_home_history, "", Button::TextureResType::PLIST );
    t_btnHistory->setScale( t_adaptation  );
    auto t_btnHistorySizeHalf = t_btnHistory->getContentSize() * t_btnHistory->getScale() * 0.5f;
    
    t_btnHistory->setPosition( Vec2( t_center.x - t_btnHistorySizeHalf.width - 20.0f, t_center.y  - t_btnHistorySizeHalf.height + 4.0f ) );
    addChild( t_btnHistory );
    
    //
    auto t_btnChineseHistory = Button::create( TexturePacker::DadGuessMain::caicai_home_chinese, TexturePacker::DadGuessMain::caicai_home_chinese, "", Button::TextureResType::PLIST );
    t_btnChineseHistory->setScale( t_adaptation );
    auto t_btnChineseHistorySizeHalf = t_btnChineseHistory->getContentSize() * t_btnChineseHistory->getScale() * 0.5f;
    
    t_btnChineseHistory->setPosition( Vec2( t_center.x + t_btnChineseHistorySizeHalf.width, t_center.y  - t_btnChineseHistorySizeHalf.height + 7.0f ) );
    addChild( t_btnChineseHistory );
    
    
    //
    auto t_btnTouched = []( Ref * p_target ){
        int t_tag = ((Button *)p_target)->getTag();
        printf( "-----> touched: %d \n", t_tag );
        
        Director::getInstance()->replaceScene( DadGuessCardListScene::createScene( "animal" ) );
    };
    
    Button * t_btnList[] = { t_btnAnimal, t_btnAbc, t_btnSky, t_btnEart, t_btnHistory, t_btnChineseHistory };
    
    for( int i = 0; i < sizeof( t_btnList ) / sizeof( Button * ); ++i )
    {
        t_btnList[i]->setTag( i );
        touchAnswer( t_btnList[i], t_btnTouched, t_adaptation * 1.2f, t_adaptation );
    }
    
    return true;
}
