//
//  Login.cpp
//  DadGuess-mobile
//
//  Created by 吴思 on 7/3/18.
//

#include "LoginScene.h"
#include "ui/CocosGUI.h"
#include "Common.h"
#include "Http.h"
#include "external/json/document.h"
#include "../DataBase/DataTableUser.h"
#include "Config.h"
#include "Message.h"
#include "../DataBase/DataValidate.h"
#include "../DadGuess/DadGuessUpdateScene.h"
#include "../DadGuess/DadGuessMainScene.h"
#include "Login.hpp"
#include "DataTableUser.h"
#include "DataTableFile.h"
#include <thread>
#include "SMSSDK.h"

USING_NS_CC;
using namespace smssdk;
using namespace cocos2d::ui;

#define PAGE_FONT "fonts/HuaKangFangYuanTIW7-GB_0.ttf"
#define PHONE_MAXLEN 13
#define PASSWORD_MAXLEN 16
#define VERIFICATIONCODE_MAXLEN 6

DataUserInfo::LoginType LoginScene::m_loginType = DataUserInfo::LoginType::phone;
bool LoginScene::m_requesting = false;

cocos2d::Scene * LoginScene::CreateScene()
{
    // static cocos2d::Scene * s_result = LoginScene::create();
    // s_result->retain();
    // return s_result;

    return LoginScene::create();
}

bool LoginScene::init()
{
    if ( !BaseScene::init() )
    {
        return false;
    }
    m_requesting = false;
    TexturePacker::Login::addSpriteFramesToCache();
    
    m_loginState = LoginState::SelectLogin;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Size windowSize = Size( visibleSize.width + origin.x, visibleSize.height + origin.y );
    
    float t_PaddingTop = ( visibleSize.height - origin.y ) * 0.15f;
    float t_PaddingButtom = ( visibleSize.height - origin.y ) * 0.18f;

    auto t_backgroud = Sprite::create( "BackgroundImage.png" );
    if (t_backgroud != nullptr)
    {
        // position the sprite on the center of the screen
        t_backgroud->setPosition( Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
        auto t_backgroundSize = t_backgroud->getContentSize();

        t_backgroud->setScale( visibleSize.width / t_backgroundSize.width, visibleSize.height / t_backgroundSize.height );
        
        this->addChild( t_backgroud, 0 );
    }
    
    auto t_title = TexturePacker::Login::createLoginTitleSprite();
    auto t_titleSize = t_title->getContentSize();
    if( t_title != nullptr )
    {
        t_title->setScale( adaptation() );
        t_title->setPosition( Vec2(visibleSize.width/2 + origin.x, visibleSize.height + origin.y - t_titleSize.height * 0.5f - t_PaddingTop ) );
        this->addChild( t_title, 1 );
    }

    m_CloundLeft = Sprite::create("Cloud.png");
    auto t_CloundLeftSize = m_CloundLeft->getContentSize();
    if( m_CloundLeft != nullptr )
    {
        m_CloundLeft->setScale( adaptation() );
        m_CloundLeft->setPosition( Vec2(visibleSize.width / 4.0f + origin.x, visibleSize.height + origin.y - t_CloundLeftSize.height * 0.5f - 5.0f - t_PaddingTop ) );
        this->addChild( m_CloundLeft, 2 );
    }

    m_CloundRight = Sprite::create("Cloud.png");
    if( m_CloundRight != nullptr )
    {
        m_CloundRight->setScale( adaptation() );
        m_CloundRight->setPosition( Vec2(visibleSize.width / 4.0f * 3.0f + origin.x, visibleSize.height + origin.y - t_CloundLeftSize.height * 0.5f - 15.0f - t_PaddingTop) );
        this->addChild( m_CloundRight, 2 );
    }

    m_back = Button::create( "Back.png", "Back.png" );
    auto t_backSize = m_back->getContentSize();
    if( m_back != nullptr )
    {
        m_back->setScale( adaptation() );
        m_back->setPosition( Vec2( origin.x + t_backSize.width * 0.5f + 20.0f, visibleSize.height + origin.y - t_backSize.height * 0.5f - 20.0f ) );
        m_back->setVisible( false );
        this->addChild( m_back );

        touchAnswer( m_back, [this]( Ref * p_ref ){
            loginBack( );
        }, adaptation() * 1.1f, adaptation() );
    }


    float t_headHeightHalf = ( m_CloundRight->getPosition().y - m_CloundRight->getContentSize().height * 0.5f ) * 0.5f;
    //选择登录方式
    {
        float t_iconPosY = t_PaddingButtom * 0.5f + t_headHeightHalf + 30.0f;
        float t_titlePosY = t_PaddingButtom * 0.5f + t_headHeightHalf - 10.0f;

        m_SelectLoginType = Layer::create();
        this->addChild( m_SelectLoginType, 2 );

        auto t_LoginWechat = Button::create( TexturePacker::Login::loginWechat, TexturePacker::Login::loginWechat, "", Widget::TextureResType::PLIST );
        if( t_LoginWechat != nullptr )
        {
            t_LoginWechat->setScale( adaptation() );
            t_LoginWechat->setPosition( Vec2( visibleSize.width / 3.0f, t_iconPosY ) );
            m_SelectLoginType->addChild( t_LoginWechat, 1 );

            touchAnswer( t_LoginWechat, [this]( Ref * p_ref ){
                loginWechat( this );
            }, adaptation() * 1.1f, adaptation() );
            
            auto t_label = Label::createWithTTF( "微信", PAGE_FONT, 12 );
            t_label->setPosition( Vec2( visibleSize.width / 3.0f, t_titlePosY ) );
            m_SelectLoginType->addChild( t_label, 2 );

        }

//        auto t_LoginSina = Button::create( TexturePacker::Login::loginSina, TexturePacker::Login::loginSina, "", Widget::TextureResType::PLIST );
//        if( t_LoginSina != nullptr )
//        {
//            t_LoginSina->setScale( adaptation() );
//            t_LoginSina->setPosition( Vec2( visibleSize.width / 4.0f * 2.0f, t_iconPosY) );
//            m_SelectLoginType->addChild(t_LoginSina, 1);
//
//            touchAnswer( t_LoginSina, [this]( Ref * p_ref ){
//                loginSina( this );
//            }, adaptation() * 1.1f, adaptation() );
//
//            auto t_label = Label::createWithTTF( "微博", PAGE_FONT, 12 );
//            t_label->setPosition( Vec2( visibleSize.width / 4.0f * 2.0f, t_titlePosY ) );
//            m_SelectLoginType->addChild( t_label, 2 );
//        }

        auto t_LoginPhone = Button::create( TexturePacker::Login::loginPhone, TexturePacker::Login::loginPhone, "", Widget::TextureResType::PLIST );
        if( t_LoginPhone != nullptr )
        {
            t_LoginPhone->setScale( adaptation() );
            t_LoginPhone->setPosition( Vec2( visibleSize.width / 3.0f * 2.0f, t_iconPosY ) );
            m_SelectLoginType->addChild(t_LoginPhone, 1);

            touchAnswer( t_LoginPhone, [this]( Ref * p_ref ){
                loginPhone( this );
            }, adaptation() * 1.1f, adaptation() );
            
            auto t_label = Label::createWithTTF( "手机", PAGE_FONT, 12 );
            t_label->setPosition( Vec2( visibleSize.width / 3.0f * 2.0f, t_titlePosY ) );
            m_SelectLoginType->addChild( t_label, 2 );
        }
    }


    //手机登陆
    auto t_eye = TexturePacker::Login::createLoginEyeCloseSprite();
    t_eye->setScale( adaptation() );
    auto t_eyeSize = t_eye->getContentSize();
    static float t_eyeOpen = false;
    
    auto t_showPassword = Label::createWithTTF( "", PAGE_FONT, 12 );
    {
        m_LoginPhone = Layer::create();
        m_LoginPhone->setVisible( false );
        this->addChild( m_LoginPhone, 1 );
        
        auto t_LoginPhoneBorder = Scale9Sprite::createWithSpriteFrame( TexturePacker::Login::createLoginBorderSprite()->getSpriteFrame() );
       float t_contentHeight = ( t_headHeightHalf * 2 - t_PaddingButtom - origin.y );

       Size t_LoginPhoneBorderSize = Size( windowSize.width * 0.5f, t_contentHeight * 0.4f );
       if( t_LoginPhoneBorder != nullptr )
       {
           t_LoginPhoneBorder->setContentSize( t_LoginPhoneBorderSize );
           t_LoginPhoneBorder->setPosition( Vec2( windowSize.width * 0.5f, t_headHeightHalf * 2 - t_LoginPhoneBorderSize.height * 0.5f - t_contentHeight * 0.1f ) );
           
           m_LoginPhone->addChild( t_LoginPhoneBorder );
       }
        
        auto t_spaceLine = DrawNode::create();
        t_LoginPhoneBorder->addChild( t_spaceLine );
        t_spaceLine->drawLine( Vec2( 15.0f, t_LoginPhoneBorderSize.height * 0.5f ), Vec2( t_LoginPhoneBorderSize.width - 15.0f, t_LoginPhoneBorderSize.height * 0.5f ), Color4F( (float)0x3A / (float)0xFF, (float)0xB5 / (float)0xFF, (float)0xFF / (float)0xFF, 1.0f ) );
        
        auto t_phoneLabel = Label::createWithTTF( "手机号", PAGE_FONT, 12 );
        auto t_phoneLabelSize = t_phoneLabel->getContentSize();
        if( t_phoneLabel != nullptr )
        {
            t_phoneLabel->setPosition( 20.0f + t_phoneLabelSize.width * 0.5f, t_LoginPhoneBorderSize.height * 0.75f - 2.0f );
            t_LoginPhoneBorder->addChild( t_phoneLabel );
        }
        
        
        
        Size t_phoneInputSize = Size( t_LoginPhoneBorderSize.width - 60.0f - t_phoneLabelSize.width, t_LoginPhoneBorderSize.height * 0.5f );
        m_loginPhoneInput = EditBox::create( t_phoneInputSize, Scale9Sprite::create( "Empty.png" ) );
        if( m_loginPhoneInput != nullptr )
        {
            m_loginPhoneInput->setFontSize( 12 );
            m_loginPhoneInput->setAnchorPoint( Point(0,0.5f) );
            m_loginPhoneInput->setTextHorizontalAlignment( TextHAlignment::LEFT );
            m_loginPhoneInput->setPosition( Vec2( t_phoneLabelSize.width + 10.0f, t_phoneLabelSize.height * 0.5f ) );
            m_loginPhoneInput->setMaxLength( PHONE_MAXLEN );
            
            m_loginPhoneInput->setInputMode( EditBox::InputMode::NUMERIC );

            t_phoneLabel->addChild( m_loginPhoneInput );
        }
        
        
        
        auto t_passwordLabel = Label::createWithTTF( "密码", PAGE_FONT, 12 );
        if( t_passwordLabel != nullptr )
        {
            auto t_passwordLabelSize = t_passwordLabel->getContentSize();
            t_passwordLabel->setPosition( 20.0f + t_passwordLabelSize.width * 0.5f, t_LoginPhoneBorderSize.height * 0.25f + 2.0f );
            t_LoginPhoneBorder->addChild( t_passwordLabel );
        }
        
        m_loginPasswordInput = EditBox::create( t_phoneInputSize, Scale9Sprite::create( "Empty.png" ) );
        if( m_loginPasswordInput != nullptr )
        {
            m_loginPasswordInput->setFontSize( 12 );
            m_loginPasswordInput->setAnchorPoint( Point(0,0.5f) );
            m_loginPasswordInput->setTextHorizontalAlignment( TextHAlignment::LEFT );
            m_loginPasswordInput->setPosition( Vec2( t_phoneLabelSize.width + 10.0f, t_phoneLabelSize.height * 0.5f - 2.0f ) );
            m_loginPasswordInput->setInputFlag( EditBox::InputFlag::PASSWORD );
            m_loginPasswordInput->setInputMode(cocos2d::ui::EditBox::InputMode::ANY);
            m_loginPasswordInput->setReturnType(EditBox::KeyboardReturnType::DONE);
            m_loginPasswordInput->setInputMode( EditBox::InputMode::SINGLE_LINE );
            m_loginPasswordInput->setMaxLength( PASSWORD_MAXLEN );
            
            t_passwordLabel->addChild( m_loginPasswordInput );
        }
        
        if( t_showPassword != nullptr )
        {
            t_showPassword->setSystemFontName( m_loginPasswordInput->getFontName() );
            t_showPassword->setAnchorPoint( Point(0,0.5f) );
            t_showPassword->setAlignment( TextHAlignment::LEFT );
            t_showPassword->setPosition( Vec2( t_phoneLabelSize.width + 15.0f, t_phoneLabelSize.height * 0.5f - 2.0f ) );
            t_showPassword->setVisible( false );
            t_passwordLabel->addChild( t_showPassword );
        }
        
        if( t_eye != nullptr )
        {
            t_eye->setPosition( Vec2( t_phoneLabelSize.width + t_phoneInputSize.width + t_eyeSize.width * 0.5f + 10.0f, t_phoneLabelSize.height * 0.5f ) );
            t_passwordLabel->addChild( t_eye );
        }

        auto t_phoneRegister = MenuItemFont::create( "手机号注册", CC_CALLBACK_1( LoginScene::toPhoneRegister, this ) );
        if( t_phoneRegister != nullptr )
        {
            t_phoneRegister->setFontSizeObj( 10 );
            auto t_phoneRegisterSize = t_phoneRegister->getContentSize();
            t_phoneRegister->setPosition( Vec2( t_phoneRegisterSize.width * 0.5f + 3.0f,  -t_phoneRegisterSize.height * 0.5f - 3.0f ) );
            auto menu = Menu::create( t_phoneRegister, NULL );
            menu->setPosition( Vec2::ZERO );
            t_LoginPhoneBorder->addChild( menu, 1 );
        }
        
        auto t_forgetPassword = MenuItemFont::create( "忘记密码？", CC_CALLBACK_1( LoginScene::toForgetPassword, this ) );
        if( t_forgetPassword != nullptr )
        {
            t_forgetPassword->setFontSizeObj( 10 );
            auto t_forgetPasswordSize = t_forgetPassword->getContentSize();
            t_forgetPassword->setPosition( Vec2( t_LoginPhoneBorderSize.width - t_forgetPasswordSize.width * 0.5f ,  -t_forgetPasswordSize.height * 0.5f - 3.0f ) );
            auto menu = Menu::create( t_forgetPassword, NULL );
            menu->setPosition( Vec2::ZERO );
            t_LoginPhoneBorder->addChild( menu, 1 );
        }

        auto t_loginButton = Button::create( TexturePacker::Login::loginButton, TexturePacker::Login::loginButton, "", Widget::TextureResType::PLIST );
        t_loginButton->setScale( adaptation() );
        auto t_loginButtonSize = t_loginButton->getContentSize();
        if( t_loginButton != nullptr )
        {
             
             float t_loginButtonMinY = t_loginButtonSize.height * 0.5f + origin.y + t_PaddingButtom;
             float t_loginButtonMaxY = t_contentHeight * 0.9 - t_LoginPhoneBorderSize.height + origin.y + t_PaddingButtom - t_loginButtonSize.height * 0.5f - 10.0f;
             float t_loginButtonBestY = ( t_contentHeight * 0.9 - t_LoginPhoneBorderSize.height ) / 3.0f + origin.y + t_PaddingButtom;

             if( t_loginButtonMinY > t_loginButtonMaxY )
             {
                 t_loginButton->setPosition( Vec2( windowSize.width * 0.5f, t_loginButtonMaxY ) );
             }else{
                t_loginButton->setPosition( Vec2( windowSize.width * 0.5f, ( t_loginButtonMinY > t_loginButtonBestY || t_loginButtonBestY > t_loginButtonMaxY ) ? t_loginButtonMinY : t_loginButtonBestY ) );
             }

            m_LoginPhone->addChild( t_loginButton, 1 );

            touchAnswer( t_loginButton, [this]( Ref * p_ref ){
                login( this );
            }, adaptation() * 1.1f, adaptation() );

         }
        
        auto t_loginLabel = Label::createWithTTF( "登陆", PAGE_FONT, 12 );
        if( t_loginLabel != nullptr )
        {
            t_loginLabel->setPosition( Vec2( t_loginButtonSize.width * 0.5f, t_loginButtonSize.height * 0.5f ) );
            t_loginLabel->setScale( 1.0f / adaptation() );
            t_loginButton->addChild( t_loginLabel );
        }
       
    }
    
    /// 注册
    {
        m_RegisterPhone = Layer::create();
        m_RegisterPhone->setVisible( false );
        this->addChild( m_RegisterPhone );
        
        auto t_RegisterPhoneBorder = Scale9Sprite::createWithSpriteFrame( TexturePacker::Login::createLoginBorderSprite()->getSpriteFrame() );
        float t_contentHeight = ( t_headHeightHalf * 2 - t_PaddingButtom - origin.y );
        Size t_RegisterPhoneBorderSize = Size( windowSize.width * 0.5f, t_contentHeight * 0.6f );
        if( t_RegisterPhoneBorder != nullptr )
        {
            t_RegisterPhoneBorder->setContentSize( t_RegisterPhoneBorderSize );
            t_RegisterPhoneBorder->setPosition( Vec2( windowSize.width * 0.5f, t_headHeightHalf * 2 - t_RegisterPhoneBorderSize.height * 0.5f - ( t_contentHeight - t_RegisterPhoneBorderSize.height ) * 0.2f ) );
            
            m_RegisterPhone->addChild( t_RegisterPhoneBorder );
        }
        
        auto t_spaceLine = DrawNode::create();
        t_RegisterPhoneBorder->addChild( t_spaceLine );
        t_spaceLine->drawLine( Vec2( 15.0f, t_RegisterPhoneBorderSize.height / 3.0f ), Vec2( t_RegisterPhoneBorderSize.width - 15.0f, t_RegisterPhoneBorderSize.height / 3.0f ), Color4F( (float)0x3A / (float)0xFF, (float)0xB5 / (float)0xFF, (float)0xFF / (float)0xFF, 1.0f ) );
        
        t_spaceLine->drawLine( Vec2( 15.0f, t_RegisterPhoneBorderSize.height / 1.5f ), Vec2( t_RegisterPhoneBorderSize.width - 15.0f, t_RegisterPhoneBorderSize.height / 1.5f ), Color4F( (float)0x3A / (float)0xFF, (float)0xB5 / (float)0xFF, (float)0xFF / (float)0xFF, 1.0f ) );
        
        auto t_phoneLabel = Label::createWithTTF( "手机号", PAGE_FONT, 12 );
        auto t_phoneLabelSize = t_phoneLabel->getContentSize();
        if( t_phoneLabel != nullptr )
        {
            t_phoneLabel->setPosition( 20.0f + t_phoneLabelSize.width * 0.5f, t_RegisterPhoneBorderSize.height / 1.2f - 2.0f );
            t_RegisterPhoneBorder->addChild( t_phoneLabel );
        }
        
        Size t_phoneInputSize = Size( t_RegisterPhoneBorderSize.width - 60.0f - t_phoneLabelSize.width, t_RegisterPhoneBorderSize.height / 3.0f );
        m_RegisterPhoneInput = EditBox::create( t_phoneInputSize, Scale9Sprite::create( "Empty.png" ) );
        if( m_RegisterPhoneInput != nullptr )
        {
            m_RegisterPhoneInput->setFontSize( 12 );
            m_RegisterPhoneInput->setAnchorPoint( Point(0,0.5f) );
            m_RegisterPhoneInput->setTextHorizontalAlignment( TextHAlignment::LEFT );
            m_RegisterPhoneInput->setPosition( Vec2( t_phoneLabelSize.width + 10.0f, t_phoneLabelSize.height * 0.5f ) );
            m_RegisterPhoneInput->setMaxLength( PHONE_MAXLEN );
            m_RegisterPhoneInput->setInputMode( EditBox::InputMode::NUMERIC );
            
            t_phoneLabel->addChild( m_RegisterPhoneInput );
        }
        
        auto t_verificationCodeLabel = Label::createWithTTF( "验证码", PAGE_FONT, 12 );
        auto t_verificationCodeLabelSize = t_verificationCodeLabel->getContentSize();
        if( t_verificationCodeLabel != nullptr )
        {
            t_verificationCodeLabel->setPosition( 20.0f + t_phoneLabelSize.width * 0.5f, t_RegisterPhoneBorderSize.height / 2.0f - 2.0f );
            t_RegisterPhoneBorder->addChild( t_verificationCodeLabel );
        }
        
        Size t_verificationCodeInputSize = Size( t_RegisterPhoneBorderSize.width - 110.0f - t_verificationCodeLabelSize.width, t_RegisterPhoneBorderSize.height / 3.0f );
        m_RegisterVerificationCodeInput = EditBox::create( t_verificationCodeInputSize, Scale9Sprite::create( "Empty.png" ) );
        if( m_RegisterVerificationCodeInput != nullptr )
        {
            m_RegisterVerificationCodeInput->setFontSize( 12 );
            m_RegisterVerificationCodeInput->setAnchorPoint( Point(0,0.5f) );
            m_RegisterVerificationCodeInput->setTextHorizontalAlignment( TextHAlignment::LEFT );
            m_RegisterVerificationCodeInput->setPosition( Vec2( t_verificationCodeLabelSize.width + 10.0f, t_verificationCodeLabelSize.height * 0.5f ) );
            
            m_RegisterVerificationCodeInput->setInputMode( EditBox::InputMode::NUMERIC );
            m_RegisterVerificationCodeInput->setMaxLength( VERIFICATIONCODE_MAXLEN );
            t_verificationCodeLabel->addChild( m_RegisterVerificationCodeInput );
        }
        
        auto t_sendVerificationCode = MenuItemFont::create( "发送验证码",  CC_CALLBACK_1( LoginScene::sendVerificationCode, this ) );
        if( t_sendVerificationCode != nullptr )
        {
            t_sendVerificationCode->setFontSizeObj( 8 );
            auto t_sendVerificationCodeSize = t_sendVerificationCode->getContentSize();
            t_sendVerificationCode->setPosition( Vec2( t_RegisterPhoneBorderSize.width - t_sendVerificationCodeSize.width * 0.5f - 20.0f, t_RegisterPhoneBorderSize.height / 2.0f ) );
            auto menu = Menu::create( t_sendVerificationCode, NULL );
            menu->setPosition( Vec2::ZERO );
            t_RegisterPhoneBorder->addChild( menu, 1 );
        }
        
        auto t_passwordLabel = Label::createWithTTF( "密码", PAGE_FONT, 12 );
        auto t_passwordLabelSize = t_passwordLabel->getContentSize();
        if( t_passwordLabel != nullptr )
        {
            t_passwordLabel->setPosition( 20.0f + t_phoneLabelSize.width * 0.5f, t_RegisterPhoneBorderSize.height / 6.0f );
            t_RegisterPhoneBorder->addChild( t_passwordLabel );
        }
        
        Size t_passwordInputSize = Size( t_RegisterPhoneBorderSize.width - 60.0f - t_verificationCodeLabelSize.width, t_RegisterPhoneBorderSize.height / 3.0f);
        m_RegisterPasswordInput = EditBox::create( t_passwordInputSize, Scale9Sprite::create( "Empty.png" ) );
        if( m_RegisterPasswordInput != nullptr )
        {
            m_RegisterPasswordInput->setFontSize( 12 );
            m_RegisterPasswordInput->setAnchorPoint( Point(0,0.5f) );
            m_RegisterPasswordInput->setTextHorizontalAlignment( TextHAlignment::LEFT );
            m_RegisterPasswordInput->setPosition( Vec2( t_verificationCodeLabelSize.width + 10.0f, t_passwordLabelSize.height * 0.5f ) );
            
            m_RegisterPasswordInput->setInputMode(cocos2d::ui::EditBox::InputMode::ANY);
            m_RegisterPasswordInput->setReturnType(EditBox::KeyboardReturnType::DONE);
            m_RegisterPasswordInput->setInputMode( EditBox::InputMode::SINGLE_LINE );
            m_RegisterPasswordInput->setMaxLength( PASSWORD_MAXLEN );
            
            t_passwordLabel->addChild( m_RegisterPasswordInput );
        }
        
        
        auto t_registerButton = Button::create( TexturePacker::Login::loginButton, TexturePacker::Login::loginButton, "", Widget::TextureResType::PLIST );
        t_registerButton->setScale( adaptation() );
        auto t_registerButtonSize = t_registerButton->getContentSize();
        if( t_registerButton != nullptr )
        {
            
            float t_registerButtonMinY = t_registerButtonSize.height * 0.5f + origin.y + t_PaddingButtom + 5.0f;
            float t_registerButtonMaxY = t_contentHeight * 0.92 - t_RegisterPhoneBorderSize.height + origin.y + t_PaddingButtom - t_registerButtonSize.height * 0.5f - 10.0f;
            float t_registerButtonBestY = ( t_contentHeight * 0.92 - t_RegisterPhoneBorderSize.height ) / 3.0f + origin.y + t_PaddingButtom;
            

            if( t_registerButtonMinY > t_registerButtonMaxY )
             {
                 t_registerButton->setPosition( Vec2( windowSize.width * 0.5f, t_registerButtonMaxY ) );
             }else{
                t_registerButton->setPosition( Vec2( windowSize.width * 0.5f, ( t_registerButtonMinY > t_registerButtonBestY || t_registerButtonBestY > t_registerButtonMaxY ) ? t_registerButtonMinY : t_registerButtonBestY ) );
            }

            m_RegisterPhone->addChild( t_registerButton, 1 );

            touchAnswer( t_registerButton, [this]( Ref * p_ref ){
                phoneRegister( this );
            }, adaptation() * 1.1f, adaptation() );

        }
        
        auto t_registerLabel = Label::createWithTTF( "注册", PAGE_FONT, 12 );
        if( t_registerLabel != nullptr )
        {
            t_registerLabel->setPosition( Vec2( t_registerButtonSize.width * 0.5f, t_registerButtonSize.height * 0.5f ) );
            t_registerLabel->setScale( 1.0f / adaptation() );
            t_registerButton->addChild( t_registerLabel, 2 );
        }
    }

    // 找回密码
    {
        m_PhoneForgetPassword = Layer::create();
        m_PhoneForgetPassword->setVisible( false );
        this->addChild( m_PhoneForgetPassword );

        auto t_ForgetPasswordBorder = Scale9Sprite::createWithSpriteFrame( TexturePacker::Login::createLoginBorderSprite()->getSpriteFrame() );
        float t_contentHeight = ( t_headHeightHalf * 2 - t_PaddingButtom - origin.y );
        Size t_ForgetPasswordBorderSize = Size( windowSize.width * 0.5f, t_contentHeight * 0.6f );
        if( t_ForgetPasswordBorder != nullptr )
        {
            t_ForgetPasswordBorder->setContentSize( t_ForgetPasswordBorderSize );
            t_ForgetPasswordBorder->setPosition( Vec2( windowSize.width * 0.5f, t_headHeightHalf * 2 - t_ForgetPasswordBorderSize.height * 0.5f - ( t_contentHeight - t_ForgetPasswordBorderSize.height ) * 0.2f ) );
            
            m_PhoneForgetPassword->addChild( t_ForgetPasswordBorder );
        }

        auto t_spaceLine = DrawNode::create();
        t_ForgetPasswordBorder->addChild( t_spaceLine );
        t_spaceLine->drawLine( Vec2( 15.0f, t_ForgetPasswordBorderSize.height / 3.0f ), Vec2( t_ForgetPasswordBorderSize.width - 15.0f, t_ForgetPasswordBorderSize.height / 3.0f ), Color4F( (float)0x3A / (float)0xFF, (float)0xB5 / (float)0xFF, (float)0xFF / (float)0xFF, 1.0f ) );
        
        t_spaceLine->drawLine( Vec2( 15.0f, t_ForgetPasswordBorderSize.height / 1.5f ), Vec2( t_ForgetPasswordBorderSize.width - 15.0f, t_ForgetPasswordBorderSize.height / 1.5f ), Color4F( (float)0x3A / (float)0xFF, (float)0xB5 / (float)0xFF, (float)0xFF / (float)0xFF, 1.0f ) );
        

        auto t_phoneLabel = Label::createWithTTF( "手机号", PAGE_FONT, 12 );
        auto t_phoneLabelSize = t_phoneLabel->getContentSize();
        if( t_phoneLabel != nullptr )
        {
            t_phoneLabel->setPosition( 20.0f + t_phoneLabelSize.width * 0.5f, t_ForgetPasswordBorderSize.height / 1.2f - 2.0f );
            t_ForgetPasswordBorder->addChild( t_phoneLabel );
        }
        
        Size t_phoneInputSize = Size( t_ForgetPasswordBorderSize.width - 60.0f - t_phoneLabelSize.width, t_ForgetPasswordBorderSize.height / 3.0f );
        m_ForgetPhoneInput = EditBox::create( t_phoneInputSize, Scale9Sprite::create( "Empty.png" ) );
        if( m_ForgetPhoneInput != nullptr )
        {
            m_ForgetPhoneInput->setFontSize( 12 );
            m_ForgetPhoneInput->setAnchorPoint( Point(0,0.5f) );
            m_ForgetPhoneInput->setTextHorizontalAlignment( TextHAlignment::LEFT );
            m_ForgetPhoneInput->setPosition( Vec2( t_phoneLabelSize.width + 10.0f, t_phoneLabelSize.height * 0.5f ) );
            m_ForgetPhoneInput->setMaxLength( PHONE_MAXLEN );
            m_ForgetPhoneInput->setInputMode( EditBox::InputMode::NUMERIC );
            
            t_phoneLabel->addChild( m_ForgetPhoneInput );
        }
        
        auto t_verificationCodeLabel = Label::createWithTTF( "验证码", PAGE_FONT, 12 );
        auto t_verificationCodeLabelSize = t_verificationCodeLabel->getContentSize();
        if( t_verificationCodeLabel != nullptr )
        {
            t_verificationCodeLabel->setPosition( 20.0f + t_phoneLabelSize.width * 0.5f, t_ForgetPasswordBorderSize.height / 2.0f - 2.0f );
            t_ForgetPasswordBorder->addChild( t_verificationCodeLabel );
        }
        
        Size t_verificationCodeInputSize = Size( t_ForgetPasswordBorderSize.width - 110.0f - t_verificationCodeLabelSize.width, t_ForgetPasswordBorderSize.height / 3.0f );
        
        m_ForgetVerificationCodeInput = EditBox::create( t_verificationCodeInputSize, Scale9Sprite::create( "Empty.png" ) );
        if( m_ForgetVerificationCodeInput != nullptr )
        {
            m_ForgetVerificationCodeInput->setFontSize( 12 );
            m_ForgetVerificationCodeInput->setAnchorPoint( Point(0,0.5f) );
            m_ForgetVerificationCodeInput->setTextHorizontalAlignment( TextHAlignment::LEFT );
            m_ForgetVerificationCodeInput->setPosition( Vec2( t_verificationCodeLabelSize.width + 10.0f, t_verificationCodeLabelSize.height * 0.5f ) );
            m_ForgetVerificationCodeInput->setInputMode( EditBox::InputMode::NUMERIC );
            m_ForgetVerificationCodeInput->setMaxLength( VERIFICATIONCODE_MAXLEN );
            t_verificationCodeLabel->addChild( m_ForgetVerificationCodeInput );
        }
        
        auto t_sendVerificationCode = MenuItemFont::create( "发送验证码",  CC_CALLBACK_1( LoginScene::sendVerificationCode, this ) );
        if( t_sendVerificationCode != nullptr )
        {
            t_sendVerificationCode->setFontSizeObj( 8 );
            auto t_sendVerificationCodeSize = t_sendVerificationCode->getContentSize();
            t_sendVerificationCode->setPosition( Vec2( t_ForgetPasswordBorderSize.width - t_sendVerificationCodeSize.width * 0.5f - 20.0f, t_ForgetPasswordBorderSize.height / 2.0f ) );
            auto menu = Menu::create( t_sendVerificationCode, NULL );
            menu->setPosition( Vec2::ZERO );
            t_ForgetPasswordBorder->addChild( menu, 1 );
        }
        
        auto t_passwordLabel = Label::createWithTTF( "新密码", PAGE_FONT, 12 );
        auto t_passwordLabelSize = t_passwordLabel->getContentSize();
        if( t_passwordLabel != nullptr )
        {
            t_passwordLabel->setPosition( 20.0f + t_phoneLabelSize.width * 0.5f, t_ForgetPasswordBorderSize.height / 6.0f );
            t_ForgetPasswordBorder->addChild( t_passwordLabel );
        }
        
        Size t_passwordInputSize = Size( t_ForgetPasswordBorderSize.width - 60.0f - t_verificationCodeLabelSize.width, t_ForgetPasswordBorderSize.height / 3.0f);
        m_ForgetPasswordInput = EditBox::create( t_passwordInputSize, Scale9Sprite::create( "Empty.png" ) );
        if( m_ForgetPasswordInput != nullptr )
        {
            m_ForgetPasswordInput->setFontSize( 12 );
            m_ForgetPasswordInput->setAnchorPoint( Point(0,0.5f) );
            m_ForgetPasswordInput->setTextHorizontalAlignment( TextHAlignment::LEFT );
            m_ForgetPasswordInput->setPosition( Vec2( t_verificationCodeLabelSize.width + 10.0f, t_passwordLabelSize.height * 0.5f ) );
            
            m_ForgetPasswordInput->setInputMode(cocos2d::ui::EditBox::InputMode::ANY);
            m_ForgetPasswordInput->setReturnType(EditBox::KeyboardReturnType::DONE);
            m_ForgetPasswordInput->setInputMode( EditBox::InputMode::SINGLE_LINE );
            m_ForgetPasswordInput->setMaxLength( PASSWORD_MAXLEN );
            
            t_passwordLabel->addChild( m_ForgetPasswordInput );
        }


        auto t_ForgetPasswordButton = Button::create( TexturePacker::Login::loginButton, TexturePacker::Login::loginButton, "", Widget::TextureResType::PLIST );
        t_ForgetPasswordButton->setScale( adaptation() );
        auto t_ForgetPasswordButtonSize = t_ForgetPasswordButton->getContentSize();
        if( t_ForgetPasswordButton != nullptr )
        {
            
            float ForgetPasswordButtonMinY = t_ForgetPasswordButtonSize.height * 0.5f + origin.y + t_PaddingButtom + 5.0f;
            float ForgetPasswordButtonMaxY = t_contentHeight * 0.92 - t_ForgetPasswordBorderSize.height + origin.y + t_PaddingButtom - t_ForgetPasswordButtonSize.height * 0.5f - 10.0f;
            float ForgetPasswordButtonBestY = ( t_contentHeight * 0.92 - t_ForgetPasswordBorderSize.height ) / 3.0f + origin.y + t_PaddingButtom;
            

            if( ForgetPasswordButtonMinY > ForgetPasswordButtonMaxY )
            {
                t_ForgetPasswordButton->setPosition( Vec2( windowSize.width * 0.5f, ForgetPasswordButtonMaxY ) );
            }else{
                t_ForgetPasswordButton->setPosition( Vec2( windowSize.width * 0.5f, ( ForgetPasswordButtonMinY > ForgetPasswordButtonBestY || ForgetPasswordButtonBestY > ForgetPasswordButtonMaxY ) ? ForgetPasswordButtonMinY : ForgetPasswordButtonBestY ) );
            }

            m_PhoneForgetPassword->addChild( t_ForgetPasswordButton, 1 );

            touchAnswer( t_ForgetPasswordButton, [this]( Ref * p_ref ){
                forgetPassword( this );
            }, adaptation() * 1.1f, adaptation() );
        }
        
        auto t_rorgetPasswordLabel = Label::createWithTTF( "确认修改", PAGE_FONT, 12 );
        if( t_rorgetPasswordLabel != nullptr )
        {
            t_rorgetPasswordLabel->setPosition( Vec2( t_ForgetPasswordButtonSize.width * 0.5f, t_ForgetPasswordButtonSize.height * 0.5f ) );
            t_rorgetPasswordLabel->setScale( 1.0f / adaptation() );
            t_ForgetPasswordButton->addChild( t_rorgetPasswordLabel, 2 );
        }

    }

    schedule( schedule_selector(LoginScene::update) );

    auto t_listener = EventListenerTouchOneByOne::create();
    t_listener->setSwallowTouches( true );
    t_listener->onTouchBegan = [=]( Touch* touch, Event* event )->bool{
        
        printf( "touch begin" );
        
        Point locationInNode = t_eye->convertToNodeSpace( touch->getLocation() );
        
        Size s = t_eye->getContentSize();
        
        Rect rect = Rect( 0, 0, s.width, s.height );
        
        if ( rect.containsPoint( locationInNode ) ) {

            t_eyeOpen = true;
            t_eye->setSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( TexturePacker::Login::loginEyeOpen ) );
            t_showPassword->setString( m_loginPasswordInput->getText() );
            m_loginPasswordInput->setVisible( false );
            t_showPassword->setVisible( true );
        }
        
        return true;
    };
    
    t_listener->onTouchEnded = [=]( Touch* touch, Event* event ){
        
        printf( "touch end \n" );
        
        Point locationInNode = t_eye->convertToNodeSpace( touch->getLocation() );

        Size s = t_eye->getContentSize();
        
        Rect rect = Rect(0, 0, s.width, s.height);
        
        if( t_eyeOpen )
        {
            t_eyeOpen = false;
            t_eye->setSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( TexturePacker::Login::loginEyeClose ) );
            t_showPassword->setVisible( false );
            m_loginPasswordInput->setVisible( true );
        }

        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority( t_listener , this );

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
    printf( "--------------> login wechat \n" );
    if( m_requesting )
    {
        return;
    }
    m_requesting = true;
    m_loginType = DataUserInfo::LoginType::wechat;
    cn::sharesdk::C2DXShareSDK::getUserInfo(cn::sharesdk::C2DXPlatTypeWeChat, LoginScene::getUserResultHandler);
}

void LoginScene::loginSina( cocos2d::Ref* pSender )
{
    printf( "--------------> login sina \n" );
    // if( m_requesting )
    // {
    //     return;
    // }
    // m_requesting = true;
    // m_loginType = DataUserInfo::LoginType::sina;
    // cn::sharesdk::C2DXShareSDK::getUserInfo(cn::sharesdk::C2DXPlatTypeSinaWeibo, LoginScene::getUserResultHandler);

    MessageBox( "暂不支持微博登陆", "敬请期待" );
}

void LoginScene::loginPhone( cocos2d::Ref* pSender )
{
    m_loginState = LoginState::PhoneLogin;
    m_SelectLoginType->setVisible( false );
    m_back->setVisible( true );
    m_LoginPhone->setVisible( true );
}

void LoginScene::loginWechatCallBack( const char * p_code )
{
    m_requesting = false;
    if( !p_code )
    {
        return;
    }

    std::map< std::string, std::string > t_parameter;
    t_parameter[ "appid" ] = "wxd6d78ef8accb5cf4";
    t_parameter[ "secret" ] = "938ae63610eea92a61f0496bec58d708";
    t_parameter[ "code" ] = p_code;
    t_parameter[ "grant_type" ] = "authorization_code";
    
    
    Http::Post( "https://api.weixin.qq.com/sns/oauth2/access_token", &t_parameter, []( Http * p_http, std::string p_res ){
        printf( " tocken: %s \n", p_res.c_str() );
        
        rapidjson::Document t_json;
        if( !ParseApiResult( t_json, p_res ) )
        {
            return;
        }
        
        std::string t_access_token = t_json["access_token"].GetString();
        std::string t_openid = t_json["openid"].GetString();
        m_requesting = false;
    }, []( Http * p_http, std::string p_res ){
        m_requesting = false;
    }, false);
    
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
        if( DadGuessUpdateScene::s_updateed )
        {
            Director::getInstance()->replaceScene( DadGuessMainScene::create() );
        }else{
            Director::getInstance()->replaceScene( DadGuessUpdateScene::create() );
        }
    });
}

void LoginScene::loginBack()
{
    switch ( m_loginState ) {
        case LoginState::PhoneLogin :
            m_back->setVisible( false );
            m_LoginPhone->setVisible( false );
            m_SelectLoginType->setVisible( true );
            m_loginState = LoginState::SelectLogin;
            break;
        case LoginState::PhoneRegister:
            m_RegisterPhone->setVisible( false );
            m_LoginPhone->setVisible( true );
            m_loginState = PhoneLogin;
            break;
        case LoginScene::PhoneForgetPassword:
            m_PhoneForgetPassword->setVisible( false );
            m_LoginPhone->setVisible( true );
            m_loginState = PhoneLogin;
        break;
        default:
            break;
    }
}

void LoginScene::toPhoneRegister( cocos2d::Ref* pSender )
{
    m_loginState = LoginState::PhoneRegister;
    m_LoginPhone->setVisible( false );
    m_RegisterPhone->setVisible( true );
}

void LoginScene::toForgetPassword( cocos2d::Ref* pSender )
{
    m_loginState = LoginState::PhoneForgetPassword;
    m_LoginPhone->setVisible( false );
    m_PhoneForgetPassword->setVisible( true );
}

void LoginScene::sendVerificationCode( cocos2d::Ref* pSender )
{

    std::string t_phone = m_RegisterPhoneInput->getText();
    if( !IsPhoneNumber( t_phone ) )
    {
        MessageBox( "请输入合法手机号", "" );
        return;
    }

    SMSSDK::getCode( SMSSDKCodeType::TextCode, t_phone.substr( t_phone.size() - 11 ), "86", "");

    printf( "-----------> aaaaa" );
}

void LoginScene::login( cocos2d::Ref* pSender )
{
    if( m_requesting )
    {
        return;
    }
    std::string t_loginPhone = m_loginPhoneInput->getText();
    std::string t_loginPassword = m_loginPasswordInput->getText();
    
    if( t_loginPhone.size() <= 0 )
    {
        MessageBox( "请输入账号密码", "" );
        return;
    }
    
    if( t_loginPassword.size() <= 0 )
    {
        MessageBox( "请输入密码", "" );
        return;
    }
    
    std::map< std::string, std::string > t_parameter;
    t_parameter[ "userMobile" ] = t_loginPhone;
    t_parameter[ "userPwd" ] = t_loginPassword;

    m_loginType = DataUserInfo::LoginType::phone;
    m_requesting = true;
    Http::Post( DOMAIN_NAME "/user/jwt/access.do", &t_parameter, [this]( Http * p_http, std::string p_res ){
        loginCallBack( p_res );
        m_requesting = false;
    }, [this]( Http * p_http, std::string p_res ){
        MessageBox( "网络异常", "" );
        m_requesting = false;
        printf( "final: %s \n", p_res.c_str() );
    }, false);
}

void LoginScene::phoneRegister( cocos2d::Ref* pSender )
{
    
    if( m_requesting )
    {
        return;
    }
    
    std::string t_phone = m_RegisterPhoneInput->getText();
    std::string t_verificationCode = m_RegisterVerificationCodeInput->getText();
    std::string t_password = m_RegisterPasswordInput->getText();
    
    if( t_phone.size() <= 0 )
    {
        MessageBox( "请输入手机号", "" );
        return;
    }
    
    if( !IsPhoneNumber( t_phone ) )
    {
        MessageBox( "请输入合法手机号", "" );
        return;
    }
    
    if( t_password.size() <= 0 )
    {
        MessageBox( "请输入密码", "" );
        return;
    }
    
    std::map< std::string, std::string > t_parameter;
    t_parameter[ "authCode" ] = t_phone;
    t_parameter[ "authType" ] = "mobile";
    t_parameter[ "authCredential" ] = t_password;
    t_parameter[ "userType" ] = "GAME_USER";
    
    t_parameter[ "loginName" ] = t_phone;
    t_parameter[ "userMobile" ] = t_phone;
    t_parameter[ "userEmail" ] = "";
    t_parameter[ "userName" ] = t_phone;
    t_parameter[ "userSex" ] = "2";
    t_parameter[ "userBirthday" ] = "";
    
    m_requesting = true;
    Http::Post( DOMAIN_NAME "/user/auth/register.do", &t_parameter, []( Http * p_http, std::string p_res ){
        rapidjson::Document t_json;

        if( !ParseApiResult( t_json, p_res ) )
        {
            return;
        }
        
        bool t_success = t_json["success"].GetBool();
        if( t_success )
        {
            MessageBox( "注册成功", "" );
            return;
        }
        
        if( !t_json.HasMember( "code" ) )
        {
            MessageBox( "未知错误", "" );
            return;
        }
        
        std::string t_code = t_json["code"].GetString();
        if( t_code == "USER_ALREADY_EXIST" )
        {
            MessageBox( "手机号已注册", "" );
            return;
        }
        m_requesting = false;
        printf( "success: %s \n", p_res.c_str() );
    }, []( Http * p_http, std::string p_res ){
        printf( "final: %s \n", p_res.c_str() );
        m_requesting = false;
    }, false );
}

void LoginScene::forgetPassword( cocos2d::Ref* pSender  )
{
    
    if( m_requesting )
    {
        return;
    }
    std::string t_phone = m_ForgetPhoneInput->getText();
    std::string t_verificationCode = m_ForgetVerificationCodeInput->getText();
    std::string t_password = m_ForgetPasswordInput->getText();
    
    if( t_phone.size() <= 0 )
    {
        MessageBox( "请输入手机号", "" );
        return;
    }
    
    if( !IsPhoneNumber( t_phone ) )
    {
        MessageBox( "请输入合法手机号", "" );
        return;
    }
    
    if( t_password.size() <= 0 )
    {
        MessageBox( "请输入密码", "" );
        return;
    }
    
    std::map< std::string, std::string > t_parameter;
    t_parameter[ "userMobile" ] = t_phone;
    t_parameter[ "userPwd" ] = t_password;
    
    m_requesting = true;
    Http::Post( DOMAIN_NAME "/game/user/updatePwd.do", &t_parameter, []( Http * p_http, std::string p_res ){
        rapidjson::Document t_json;
        
        if( !ParseApiResult( t_json, p_res ) )
        {
            return;
        }
        
        bool t_success = t_json["success"].GetBool();
        if( t_success )
        {
            MessageBox( "修改密码成功", "" );
            return;
        }
        
        if( !t_json.HasMember( "code" ) )
        {
            MessageBox( "未知错误", "" );
            return;
        }
        
        std::string t_code = t_json["code"].GetString();
        m_requesting = false;
        printf( "success: %s \n", p_res.c_str() );
    }, []( Http * p_http, std::string p_res ){
        printf( "final: %s \n", p_res.c_str() );
        m_requesting = false;
    }, false );
}


bool LoginScene::isLogined( void )
{
    return DataTableUser::instance().getActivation().userId.length() > 0;
}

LoginScene::~LoginScene()
{
    TexturePacker::Login::removeSpriteFramesFromCache();
}


void LoginScene::getUserResultHandler(int reqID, cn::sharesdk::C2DXResponseState state, cn::sharesdk::C2DXPlatType platType, __Dictionary *result)
{

    if( state != cn::sharesdk::C2DXResponseState::C2DXResponseStateSuccess )
    {
        printf( "----------------> error state %d \n", (int)state );
        m_requesting = false;
        return;
    }
    
    std::string t_result = toString( *result );
    
    std::string t_loginType = "";
    
    rapidjson::Document t_json;
    std::string t_sendStr = "";
    
    t_json.Parse( t_result.c_str() );
    
    if( t_json.HasParseError() )
    {
        m_requesting = false;
        return;
    }
    
    std::stringstream t_sstr;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    auto t_Dict = cn::sharesdk::C2DXShareSDK::getAuthInfo( cn::sharesdk::C2DXPlatTypeWeChat );
    DictElement * t_Element;
    CCDICT_FOREACH(t_Dict, t_Element)
    {
        const char * key = t_Element->getStrKey();
        
        CCString * value = (CCString *)t_Element->getObject();

        printf( "-----------> %s : %s \n", key, value->getCString() );
    }

    switch ( platType ) {
        case cn::sharesdk::C2DXPlatType::C2DXPlatTypeWeChat:
            t_loginType = "weixin";
            
            //weixin:openid ,token(access_token), refresh_token,expiresIn,unionid;

            t_sstr << "{";
            t_sstr << "\"openid\": \"" << t_json["openid"].GetString() << "\", ";
            t_sstr << "\"token\": \"" << ((CCString *)t_Dict->objectForKey( "token" ))->getCString() << "\", ";
            t_sstr << "\"refresh_token\": \"" << "\", ";
            t_sstr << "\"expiresIn\": \"" << 0 << "\", ";
            t_sstr << "\"headimgurl\": \"" << ((CCString *)t_Dict->objectForKey( "userIcon" ))->getCString() << "\", ";
            t_sstr << "\"unionid\": \"" << ((CCString *)t_Dict->objectForKey( "unionID" ))->getCString() << "\"";
            t_sstr << "}";
            
            t_sendStr = t_sstr.str();
            
            break;
        case cn::sharesdk::C2DXPlatType::C2DXPlatTypeSinaWeibo:
            t_loginType = "weibo";
            break;
        default:
            break;
    }
#else
    auto t_credential = t_json["credential"].GetObject();
    
    switch ( platType ) {
        case cn::sharesdk::C2DXPlatType::C2DXPlatTypeWeChat:
            t_loginType = "weixin";
            
            //weixin:openid ,token(access_token), refresh_token,expiresIn,unionid;

            t_sstr << "{";
            t_sstr << "\"openid\": \"" << t_json["openid"].GetString() << "\", ";
            t_sstr << "\"token\": \"" << t_credential["access_token"].GetString() << "\", ";
            t_sstr << "\"refresh_token\": \"" << t_credential["refresh_token"].GetString() << "\", ";
            t_sstr << "\"expiresIn\": \"" << t_credential["expires_in"].GetString() << "\", ";
            t_sstr << "\"unionid\": \"" << t_credential["unionid"].GetString() << "\"";
            t_sstr << "}";
            
            t_sendStr = t_sstr.str();
            
            break;
        case cn::sharesdk::C2DXPlatType::C2DXPlatTypeSinaWeibo:
            t_loginType = "weibo";
            break;
        default:
            break;
    }
#endif
    
    std::map< std::string, std::string > t_parameter;
    t_parameter[ "userType" ] = "GAME_USER";
    t_parameter[ "data" ] = t_sendStr;
    
    
    switch (state)
    {
        case cn::sharesdk::C2DXResponseStateSuccess:
        {
            Http::Post( std::string( DOMAIN_NAME "/user/auth/" ) + t_loginType +  "/access.do", &t_parameter, []( Http * p_http, std::string p_res ){
                loginCallBack( p_res );
            }, []( Http * p_http, std::string p_res ){
                MessageBox( "网络异常", "" );
            }, false);
        }
            break;
        case cn::sharesdk::C2DXResponseStateFail:
        {
            //回调错误信息
            MessageBox( "登陆异常", "" );
        }
            break;
        case cn::sharesdk::C2DXResponseStateCancel:
        {
//            log("Cancel");
        }
            break;
        default:
            break;
    }
}


void LoginScene::loginCallBack( const std::string & p_str )
{

    printf( "------------> loginCallBack %s \n", p_str.c_str() );
    rapidjson::Document t_json;
    if( !ParseApiResult( t_json, p_str ) )
    {
        return;
    }

    bool t_success = t_json["success"].GetBool();

    if( t_success && t_json.HasMember( "data" ) )
    {
        auto t_data = t_json["data"].GetObject();
        
        if( t_data.HasMember( "user" ) && t_data.HasMember( "token" )  )
        {
            auto t_user = t_data["user"].GetObject();
            
            DataUserInfo t_dataUser;
            
            t_dataUser.userId = t_user["userId"].GetString();
            t_dataUser.userName = t_user["userName"].GetString();
            t_dataUser.loginName = t_user["loginName"].GetType() == rapidjson::kNullType ? "" : t_user["loginName"].GetString();
            t_dataUser.userBirthday = t_user["userBirthday"].GetType() == rapidjson::kNullType ? "" : t_user["userBirthday"].GetString();
            t_dataUser.headImg = "";
            t_dataUser.userSex = t_user["userSex"].GetInt();
            t_dataUser.activation = 1;
            t_dataUser.token = t_data["token"].GetString();
            t_dataUser.loginType = m_loginType;

            std::string t_downloadUrl = t_user["headImg"].GetType() == rapidjson::kNullType ? "" : t_user["headImg"].GetString();

            if( !t_downloadUrl.empty() )
            {
                if( t_downloadUrl.find( "http" ) == std::string::npos )
                {
                    t_downloadUrl = std::string( DOMAIN_NAME ) + "/" + t_downloadUrl;
                }

                printf( "-----------------> %s \n", t_downloadUrl.c_str() );

                DataFileInfo t_DataFileInfo = DataTableFile::instance().findBySourceUrl( t_downloadUrl );
                if( t_DataFileInfo.fileId.empty() )
                {
                    Http::DownloadFile( t_downloadUrl, "png", [t_dataUser]( Http * p_http, DataFileInfo p_fineInfo ){
                        DataUserInfo t_updateUser = t_dataUser;
                        t_updateUser.headImg = p_fineInfo.fileId;

                        DataTableUser::instance().update( t_updateUser );

                    },[t_dataUser]( Http * p_http, DataFileInfo p_fileInfo ){
                        DataTableUser::instance().remove( t_dataUser.userId );
                    } );
                }else{
                    t_dataUser.headImg = t_DataFileInfo.fileId;
                }
            }
            
            if( DataTableUser::instance().find( t_dataUser.userId ).userId == t_dataUser.userId  )
            {
                DataTableUser::instance().update( t_dataUser );
            }else{
                DataTableUser::instance().insert( t_dataUser );
            }
            
            Http::token = t_dataUser.token;
            
            printf( "token: %s \n", Http::token.c_str() );
            std::map< std::string, std::string > t_parameter;
            Http::Post( DOMAIN_NAME "/wechat/app/updateUserInfo.do", &t_parameter, []( Http * p_http, std::string p_res ){
                printf( "------------> %s \n", p_res.c_str() );
            }, []( Http * p_http, std::string p_res ){
                
            }, false);

            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
                if( DadGuessUpdateScene::s_updateed )
                {
                    Director::getInstance()->replaceScene( DadGuessMainScene::create() );
                }else{
                    Director::getInstance()->replaceScene( DadGuessUpdateScene::create() );
                }
            });

            return;
        }
    }
    
    if( !t_json.HasMember( "code" ) )
    {
        MessageBox( "未知错误", "" );
        return;
    }
    
    std::string t_code = t_json["code"].GetString();
    
    if( t_code == "USER_ERROR_PASSWORD" || t_code == "USER_NOT_FOUND" )
    {
        MessageBox( "账号或密码错误", "" );
        return;
    }
    
    if( t_json.HasMember( "msg" ) )
    {
        std::string t_msg = t_json["msg"].GetString();
        MessageBox( t_msg.c_str(), "" );
    }

}

