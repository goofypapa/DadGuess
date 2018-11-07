#include "CadenceGameMainScene.h"
#include "CadenceMain.hpp"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Common.h"
#include "MainScene.h"
#include "CadenceGameLoaderScene.h"

USING_NS_CC;
using namespace cocos2d::ui;

bool CadenceGameMainScene::init( void )
{
    if( !BaseScene::init() )
    {
        return false;
    }
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile( "Cadence/CadenceMain.plist", Director::getInstance()->getTextureCache()->getTextureForKey( "Cadence/CadenceMain.png" ) );

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto t_backgroud = TexturePacker::CadenceMain::createCadenceBackground_1Sprite();
    if (t_backgroud != nullptr)
    {
        t_backgroud->setPosition( Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y) );
        auto t_backgroundSize = t_backgroud->getContentSize();
        
        t_backgroud->setScale( visibleSize.width / t_backgroundSize.width, visibleSize.height / t_backgroundSize.height );
        
        this->addChild( t_backgroud, 0 );
    }

    auto t_back = Button::create( "Back.png" );
    auto t_backSizeHalf = t_back->getContentSize() * 0.5f;
    t_back->setScale( adaptation() );
    t_back->setPosition( Vec2( origin.x + t_backSizeHalf.width + 20.0f, origin.y + visibleSize.height - t_backSizeHalf.height - 20.0f ) );

    this->addChild( t_back );

    touchAnswer( t_back, []( Ref * p_ref ){
        Director::getInstance()->replaceScene( MainScene::create() );
    }, adaptation() * 1.1f, adaptation() );
    

    return true;
}

CadenceGameMainScene::~CadenceGameMainScene()
{
    CadenceGameLoaderScene::uncacheResource();
}
