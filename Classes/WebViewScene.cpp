//
//  WebViewScene.cpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/8/2.
//

#include "WebViewScene.h"
#include "Common.h"

USING_NS_CC;

bool WebViewScene::init( void )
{
    if( !Scene::init() )
    {
        return false;
    }
    
//    setAppOrientations( true );
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    m_webview = experimental::ui::WebView::create();
    
    m_webview->setContentSize( visibleSize );
    m_webview->setPosition( Vec2( origin.x + visibleSize.width * 0.5f, origin.y + visibleSize.height * 0.5f ) );
    
    m_webview->loadURL( "http://www.dadpat.com/dist/dadpat/index.html" );
//    m_webview->loadURL( "https://www.baidu.com" );
//    m_webview->set

    
    this->addChild( m_webview );

    return true;
}


