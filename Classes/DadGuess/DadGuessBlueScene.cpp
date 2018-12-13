#include "DadGuessBlueScene.h"
#include "Common.h"

USING_NS_CC;

bool DadGuessBlueScene::init( void )
{
    if( !BaseScene::init() )
    {
        return false;
    }

    auto t_visibleSizeHalf = Director::getInstance()->getVisibleSize() * 0.5f;
    Vec2 t_origin = Director::getInstance()->getVisibleOrigin();

    auto t_background = DrawNode::create();
    t_background->drawSolidRect( Vec2::ZERO , t_visibleSizeHalf * 2.0f, createColor4FWithStr( "9ed667" ) );
    
    addChild( t_background );

    //7ad4f9

    return true;
}