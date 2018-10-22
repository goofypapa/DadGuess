//
//  WebViewScene.cpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/8/2.
//

#include "WebViewScene.h"
#include "Common.h"
#include "MainScene.h"
#include <sstream>
#include "Http.h"

USING_NS_CC;
using namespace cocos2d::ui;


cocos2d::Scene * WebViewScene::createWithUrl( const std::string & p_url, const bool p_orientation )
{
    auto t_res = create();
    
    if( !t_res->initWithUrl( p_url, p_orientation) ){
        delete t_res;
        t_res = nullptr;
    }
    
    return t_res;
}

bool WebViewScene::init( void )
{
    m_firstLoad = true;
    return true;
}


bool WebViewScene::initWithUrl( const std::string & p_url, const bool p_orientation )
{
    m_webOrientation = p_orientation;
    
    if( m_webOrientation ){
        setAppOrientation( true );
    }
    
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
    
    std::stringstream t_splistName;
    
    t_splistName << "Animate" << p_url << ".plist";
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile( t_splistName.str() );
    Animation * t_animation = Animation::create();
    for( int i = 0; i < 25; ++i )
    {
        std::stringstream t_spriteName;
        t_spriteName << p_url << "_" << ( i + 1 ) << ".png";
        auto t_frame = SpriteFrameCache::getInstance()->getSpriteFrameByName( t_spriteName.str() );
        t_animation->addSpriteFrame( t_frame );
    }
    
    t_animation->setDelayPerUnit(0.04f);
    t_animation->setLoops( -1 );
    
    Animate * t_animate = Animate::create( t_animation );
    
    auto t_run = Sprite::create();
    t_run->setPosition( Vec2( origin.x + visibleSize.width * 0.5f, origin.y + visibleSize.height * 0.5f ) );
    
    addChild( t_run );
    t_run->runAction( t_animate );
    
    
    m_webview = experimental::ui::WebView::create();

    m_webview->setContentSize( Size( visibleSize.width, visibleSize.height ) );

    m_webview->setPosition( Vec2( origin.x + visibleSize.width * 0.5f, origin.y + visibleSize.height * 0.5f ) );
//    m_webview->loadURL( p_url );
    
    std::stringstream t_surl;
    t_surl << "Web/" << p_url << "/index.html";
    
    m_webview->loadFile( t_surl.str() );


    m_webview->setJavascriptInterfaceScheme( "goofypapa" );
    m_webview->setBounces(false);

    m_webview->setOnDidFinishLoading([this]( experimental::ui::WebView * p_scene, std::string p_url ){

        printf("---------------> setOnDidFinishLoading");

        if( m_firstLoad ){
            m_webview->setOpacityWebView( 0.0f );
            m_webview->runAction( ActionFloat::create( 0.2f, 0.0f, 1.0f, [this]( float p_val){
                m_webview->setOpacityWebView( p_val );
            }) );
            m_firstLoad = false;
        }
        
        std::stringstream t_sstrJsCode;
        t_sstrJsCode << "window.goofypapaGame = true;"  << "window.goofypapaToken=function(){return \"" << Http::token << "\";};";
        m_webview->evaluateJS( t_sstrJsCode.str() );

//        m_webview->evaluateJS( "alert(window.goofypapaGame);" );
//        m_webview->evaluateJS( "alert(window.goofypapaToken);" );
    });

    m_webview->setOnJSCallback( [this]( experimental::ui::WebView * p_scene, std::string p_url ){
        auto t_list = split( p_url, "://" );
        if( t_list.size() == 2 && t_list[1] == "back" )
        {
            if( m_webOrientation ){
                setAppOrientation( false );
            }
            Director::getInstance()->replaceScene( MainScene::create() );
        }
        
    } );
    
    m_webview->setOpacityWebView( 0.0f );
    this->addChild( m_webview );
    
    return true;
}

