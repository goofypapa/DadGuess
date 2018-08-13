//
//  WebViewScene.cpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/8/2.
//

#include "WebViewScene.h"
#include "Common.h"
#include "MainScene.h"

USING_NS_CC;
using namespace cocos2d::ui;

bool WebViewScene::init( void )
{
    
    setAppOrientation( true );
    
    if( !Scene::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    printf( "visibleSize.width: %f, visibleSize.height: %f \n", visibleSize.width, visibleSize.height );
    printf( "origin.x: %f, origin.y: %f \n", origin.x, origin.y );
    
    auto contentSize = getContentSize();
    printf( "contentSize.width: %f, contentSize.height: %f \n", contentSize.width, contentSize.height );

    m_webview = experimental::ui::WebView::create();

    m_webview->setContentSize( Size( visibleSize.width, visibleSize.height ) );

    m_webview->setPosition( Vec2( origin.x + visibleSize.width * 0.5f, origin.y + visibleSize.height * 0.5f ) );
    m_webview->loadURL( "http://www.dadpat.com/dist/dadpat/index.html" );
    
    m_webview->evaluateJS( "window.goofypapaGame = true;" );
    m_webview->setJavascriptInterfaceScheme( "goofypapa" );
    m_webview->setOnJSCallback( []( experimental::ui::WebView * p_scene, std::string p_url ){
        auto t_list = split( p_url, "://" );
        if( t_list.size() == 2 && t_list[1] == "back" )
        {
            setAppOrientation( false );
            Director::getInstance()->replaceScene( MainScene::create() );
        }
    } );

    this->addChild( m_webview );
    
    
    
//    DrawNode * t_testRect = DrawNode::create();
//
//    t_testRect->drawSolidRect( Vec2( 0.0f, 0.0f ), Vec2( 10.0f, 10.0f ), Color4F( 1.0f, 0.0f, 0.0f, 1.0f ) );
//
//    t_testRect->setPosition( Vec2(  10.0f ,  10.0f ) );
//
//    this->addChild( t_testRect );
    
//    auto t_node = Node::create();
//
//    this->addChild( t_node );
//
//    auto t_button = Button::create( "Back.png", "Back.png" );
//    auto t_buttonSize = t_button->getContentSize();
//
//    t_button->setPosition( Vec2(  t_buttonSize.width * 0.5f,  t_buttonSize.height * 0.5f ) );
//
//    t_node->addChild( t_button, 10 );
    
    

    return true;
}


