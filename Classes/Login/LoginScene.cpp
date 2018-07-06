//
//  Login.cpp
//  dadpat-mobile
//
//  Created by 吴思 on 7/3/18.
//

#include "LoginScene.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

cocos2d::Scene * LoginScene::CreateScene()
{
    // static cocos2d::Scene * s_result = LoginScene::create();
    // s_result->retain();
    // return s_result;

    return LoginScene::create();
}

bool LoginScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    printf( "visibleSize: {%f, %f} \n", visibleSize.width, visibleSize.height );
    printf( "origin: {%f, %f} \n", origin.x, origin.y );
    
    float t_PaddingTop = ( visibleSize.height + origin.y ) * 0.1f;
    float t_PaddingButtom = ( visibleSize.height + origin.y ) * 0.1f;

    auto t_backgroud = Sprite::create( "LoginBG.png" );
    if (t_backgroud != nullptr)
    {
        // position the sprite on the center of the screen
        t_backgroud->setPosition( Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
        auto t_backgroundSize = t_backgroud->getContentSize();

        t_backgroud->setScale( visibleSize.width / t_backgroundSize.width, visibleSize.height / t_backgroundSize.height );
        
        this->addChild( t_backgroud, 0 );
    }
    
    auto t_title = Sprite::create( "LoginTitle.png" );
    auto t_titleSize = t_title->getContentSize();
    if( t_title != nullptr )
    {
        t_title->setPosition( Vec2(visibleSize.width/2 + origin.x, visibleSize.height + origin.y - t_titleSize.height * 0.5f - t_PaddingTop ) );
        this->addChild( t_title, 1 );
    }

    m_CloundLeft = Sprite::create("LoginCloud.png");
    auto t_CloundLeftSize = m_CloundLeft->getContentSize();
    if( m_CloundLeft != nullptr )
    {
        m_CloundLeft->setPosition( Vec2(visibleSize.width / 4.0f + origin.x, visibleSize.height + origin.y - t_CloundLeftSize.height * 0.5f - t_PaddingTop ) );
        this->addChild( m_CloundLeft, 2 );
    }

    m_CloundRight = Sprite::create("LoginCloud.png");
    if( m_CloundRight != nullptr )
    {
        m_CloundRight->setPosition( Vec2(visibleSize.width / 4.0f * 3.0f + origin.x, visibleSize.height + origin.y - t_CloundLeftSize.height * 0.5f - 20.0f - t_PaddingTop) );
        this->addChild( m_CloundRight, 2 );
    }


    auto t_back = MenuItemImage::create( "Back.png", "Back.png", CC_CALLBACK_1( LoginScene::loginBack, this ) );
    if( t_back != nullptr )
    {
        t_back->setPosition( Vec2( 30.0f, visibleSize.height + 15.0f ) );
        m_back = Menu::create(t_back, NULL);
        m_back->setPosition(Vec2::ZERO);
        m_back->setVisible( false );
        this->addChild(m_back, 1);
    }


    //选择登录方式
    {
        m_SelectLoginType = Layer::create();
        this->addChild( m_SelectLoginType, 2 );

        auto t_LoginWechat = MenuItemImage::create( "LoginWechat.png", "LoginWechat.png", CC_CALLBACK_1( LoginScene::loginWechat, this ) );
        if( t_LoginWechat != nullptr )
        {
            t_LoginWechat->setPosition( Vec2( visibleSize.width / 4.0f, visibleSize.height * 0.7f ) );
            auto menu = Menu::create(t_LoginWechat, NULL);
            menu->setPosition(Vec2::ZERO);
            m_SelectLoginType->addChild(menu, 1);
            
            auto t_label = Label::createWithTTF( "微信", "fonts/HuaKangFangYuanTIW7-GB_0.ttf", 12 );
            t_label->setPosition( Vec2( visibleSize.width / 4.0f, visibleSize.height * 0.7f - 40.0f ) );
            m_SelectLoginType->addChild( t_label, 2 );

        }

        auto t_LoginSina = MenuItemImage::create( "LoginSina.png", "LoginSina.png", CC_CALLBACK_1( LoginScene::loginSina, this ) );
        if( t_LoginSina != nullptr )
        {
            t_LoginSina->setPosition( Vec2( visibleSize.width / 4.0f * 2.0f, visibleSize.height * 0.7f ) );
            auto menu = Menu::create(t_LoginSina, NULL);
            menu->setPosition(Vec2::ZERO);
            m_SelectLoginType->addChild(menu, 1);
            
            auto t_label = Label::createWithTTF( "微博", "fonts/HuaKangFangYuanTIW7-GB_0.ttf", 12 );
            t_label->setPosition( Vec2( visibleSize.width / 4.0f * 2.0f, visibleSize.height * 0.7f - 40.0f ) );
            m_SelectLoginType->addChild( t_label, 2 );
        }

        auto t_LoginPhone = MenuItemImage::create( "LoginPhone.png", "LoginPhone.png", CC_CALLBACK_1( LoginScene::loginPhone, this ) );
        if( t_LoginPhone != nullptr )
        {
            t_LoginPhone->setPosition( Vec2( visibleSize.width / 4.0f * 3.0f, visibleSize.height * 0.7f ) );
            auto menu = Menu::create(t_LoginPhone, NULL);
            menu->setPosition(Vec2::ZERO);
            m_SelectLoginType->addChild(menu, 1);
            
            auto t_label = Label::createWithTTF( "手机", "fonts/HuaKangFangYuanTIW7-GB_0.ttf", 12 );
            t_label->setPosition( Vec2( visibleSize.width / 4.0f * 3.0f, visibleSize.height * 0.7f - 40.0f ) );
            m_SelectLoginType->addChild( t_label, 2 );
        }
    }

    //手机登陆
    {
        m_LoginPhone = Layer::create();
        this->addChild( m_LoginPhone, 1 );
        
       auto t_LoginPhoneBorder = Scale9Sprite::create( "LoginBorder.png" );
       
    }

    schedule( schedule_selector(LoginScene::update) );

    return true;
}

void LoginScene::update( float p_delta )
{
    static float s_date = 0.0f;
    s_date += p_delta;
    if( m_CloundLeft != nullptr )
    {
        static Vec2 s_CloundLeftPos = m_CloundLeft->getPosition();
        m_CloundLeft->setPosition( s_CloundLeftPos + sinf( s_date ) * Vec2( 10.0f, 0.0f )  );
    }
    
    if( m_CloundRight != nullptr )
    {
        static Vec2 s_CloundRightPos = m_CloundRight->getPosition();
        m_CloundRight->setPosition( s_CloundRightPos + cosf( s_date * 1.1f ) * Vec2( 15.0f, 0.0f ) );
    }
    
}

void LoginScene::loginWechat( cocos2d::Ref* pSender )
{

}

void LoginScene::loginSina( cocos2d::Ref* pSender )
{

}

void LoginScene::loginPhone( cocos2d::Ref* pSender )
{

    m_SelectLoginType->setVisible( false );
    m_back->setVisible( true );

    // Scene * t_scene = LoginPhoneScene::CreateScene();

    // Director::getInstance()->replaceScene( TransitionSlideInR::create( 0.3f, t_scene ) );
}

void LoginScene::loginBack( cocos2d::Ref* pSender )
{
    m_SelectLoginType->setVisible( true );
    m_back->setVisible( false );
}
