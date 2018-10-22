//
//  DialogPersonalCenter.cpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/8/9.
//

#include "DialogPersonalCenterLayer.h"
#include "Dialog.hpp"
#include "Common.h"
#include "LoginScene.h"
#include "Http.h"
#include "DataTableFile.h"
#include "Config.h"

USING_NS_CC;
using namespace cocos2d::ui;

#define PAGE_FONT "fonts/HuaKangFangYuanTIW7-GB_0.ttf"

bool DialogPersonalCenterLayer::init( void )
{
    if( !Layer::init() )
    {
        return false;
    }

    m_loginUser = DataTableUser::instance().getActivation();

    hideCallBack = nullptr;
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    float fontScale = 1.0f / adaptation();
    
    m_vague = DrawNode::create();
    m_vague->setPosition( -Vec2( visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y ) );
    m_vague->drawSolidRect( origin, origin + visibleSize, Color4F( 0.0f, 0.0f, 0.0f, 0.6f ) );
    m_vague->setVisible( false );
    this->addChild( m_vague );
    
    m_dialogCentBackground = TexturePacker::Dialog::createPersonalCenterBackgroundSprite();
    auto t_DialogCentBackgroundSize = m_dialogCentBackground->getContentSize();
    this->addChild(m_dialogCentBackground);
    
    m_dialogCentSaveButton = Button::create( TexturePacker::Dialog::saveButton, TexturePacker::Dialog::saveButton, "", TextureResType::PLIST );
    auto t_DialogCentSaveButtonSize = m_dialogCentSaveButton->getContentSize();
    
    m_dialogCentSaveButton->setScale( adaptation() );
    
    touchAnswer( m_dialogCentSaveButton, [this]( Ref * p_ref ){
        hide();
    }, adaptation() * 1.2f, adaptation() );
    
    float t_DialogCentSaveButtonHeight = t_DialogCentSaveButtonSize.height * adaptation();
    this->setPosition( Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y + t_DialogCentSaveButtonHeight * 0.5f ) );
    float t_DialogCentBackgroundHeight = visibleSize.height - ( t_DialogCentSaveButtonHeight * 1.6f ) * 2.0f;
    
    m_dialogCentBackgroundHiedPos = Vec2( 0.0f, visibleSize.height * 0.5f + t_DialogCentBackgroundHeight * 0.5f );
    m_dialogCentBackgroundShowPos = Vec2( 0.0f, t_DialogCentSaveButtonHeight * 0.5f );
    
    m_dialogCentSaveButtonHidePos = Vec2( 0.0f, -(visibleSize.height * 0.5f + t_DialogCentSaveButtonHeight * 0.5f) );
    m_dialogCentSaveButtonShowPos = Vec2( 0.0f, -(visibleSize.height * 0.5f - t_DialogCentSaveButtonHeight * 1.3f) );
    
    m_dialogCentBackground->setPosition( m_dialogCentBackgroundHiedPos );
    m_dialogCentSaveButton->setPosition( m_dialogCentSaveButtonHidePos );
    this->addChild( m_dialogCentSaveButton );
    
    m_dialogCentBackground->setScale( t_DialogCentBackgroundHeight / t_DialogCentBackgroundSize.height );
    
    m_dialogCentBackground->setVisible(false);
    m_dialogCentSaveButton->setVisible(false);
    
    float t_padding = t_DialogCentBackgroundSize.width * 0.06f;
    
    auto t_personalHeadBackground = Button::create( TexturePacker::Dialog::personalHeadBackground, TexturePacker::Dialog::personalHeadBackground, "", TextureResType::PLIST );
    auto t_personalHeadBackgroundSizeHalf = t_personalHeadBackground->getContentSize() * 0.5f;
    t_personalHeadBackground->setPosition( Vec2( t_personalHeadBackgroundSizeHalf.width + t_padding, t_DialogCentBackgroundSize.height - t_personalHeadBackgroundSizeHalf.height - t_padding ) );
    m_dialogCentBackground->addChild( t_personalHeadBackground );

    touchAnswer( t_personalHeadBackground, [this]( Ref * p_ref ){


    } );

    auto t_personalHeadBorder = TexturePacker::Dialog::createPersonalHeadBorderSprite();
    auto t_personalHeadBorderSizeHalf = t_personalHeadBorder->getContentSize() * 0.5f;

    t_personalHeadBorder->setScale( t_personalHeadBackgroundSizeHalf.width * 0.82f / t_personalHeadBorderSizeHalf.width );
    t_personalHeadBorder->setPosition( Vec2( t_personalHeadBackgroundSizeHalf.width, t_personalHeadBackgroundSizeHalf.height ) );
    t_personalHeadBackground->addChild( t_personalHeadBorder );

    
    auto t_fileInfo = DataTableFile::instance().find( m_loginUser.headImg );
    auto t_personalHead = t_fileInfo.fileId.empty() ? Sprite::create( "DefaultHead.png" ) : Sprite::create( t_fileInfo.fileName );
    auto t_personalHeadSizeHalf = t_personalHead->getContentSize() * 0.5f;

    t_personalHead->setScale( t_personalHeadBackgroundSizeHalf.width * 0.8f / t_personalHeadSizeHalf.width );

    t_personalHead->setPosition( Vec2( t_personalHeadBackgroundSizeHalf.width, t_personalHeadBackgroundSizeHalf.height ) );
    t_personalHeadBackground->addChild( t_personalHead );

    GLProgram * t_userHeadProgram = GLProgramCache::getInstance()->getGLProgram( "UserHead" );
    if( !t_userHeadProgram )
    {
        t_userHeadProgram = GLProgram::createWithFilenames( "UserHead.vert", "UserHead.frag" );
        GLProgramCache::getInstance()->addGLProgram( t_userHeadProgram, "UserHead" );
    }

    GLProgramState * t_userHeadProgramState = GLProgramState::getOrCreateWithGLProgram( t_userHeadProgram );
    t_personalHead->setGLProgramState( t_userHeadProgramState );
    
    
    float t_listPosX = t_personalHeadBackgroundSizeHalf.width * 2.0f + t_padding * 1.6f;
    
    auto t_nameLabel = Label::createWithTTF( "姓名:", PAGE_FONT, 16 );
    auto t_nameLabelSizeHalf = t_nameLabel->getContentSize() * 0.5f * fontScale;
    
    auto t_nameEditBackground = TexturePacker::Dialog::createTexteditBackgroundSprite();
    auto t_nameEditBackgroundSizeHalf = t_nameEditBackground->getContentSize() * 0.5f;
    
    t_nameLabel->setScale( fontScale );
    t_nameLabel->setTextColor( Color4B( 0xFF, 0xFF, 0xFF, 0xFF ) );
    t_nameLabel->enableOutline( Color4B( 0xC6, 0x72, 0x0C, 0xFF ), 2 );

    t_nameLabel->setPosition( Vec2( t_listPosX + t_nameLabelSizeHalf.width, t_DialogCentBackgroundSize.height - t_padding * 1.2f - t_nameEditBackgroundSizeHalf.height ) );
    m_dialogCentBackground->addChild( t_nameLabel );
    
    t_nameEditBackground->setPosition( Vec2( t_listPosX + t_nameLabelSizeHalf.width * 2.0f + t_padding * 0.6f + t_nameEditBackgroundSizeHalf.width, t_DialogCentBackgroundSize.height - t_padding * 1.2f - t_nameEditBackgroundSizeHalf.height  ) );
    m_dialogCentBackground->addChild( t_nameEditBackground );
    
    float t_nameEditMargin = t_nameEditBackgroundSizeHalf.width * 0.1f;
    m_nameEditBoxShowPos = convertToNodeSpace( t_nameEditBackground->convertToWorldSpace( Vec2( t_nameEditMargin, t_nameEditBackgroundSizeHalf.height ) ) ) - Vec2( 0.0f, m_dialogCentBackgroundHiedPos.y - m_dialogCentBackgroundShowPos.y );
    m_nameEditBoxHidePos = convertToNodeSpace( t_nameEditBackground->convertToWorldSpace( Vec2( t_nameEditMargin, t_nameEditBackgroundSizeHalf.height ) ) );
    
    m_nameEditBox = EditBox::create( Size( ( t_nameEditBackgroundSizeHalf.width - t_nameEditMargin ) * 1.6f * adaptation(), t_nameEditBackgroundSizeHalf.height * 2.0f ), Scale9Sprite::create( "Empty.png" ) );

    m_nameEditBox->setFontSize( 9 );
    m_nameEditBox->setAnchorPoint( Point( 0, 0.5f ) );
    m_nameEditBox->setTextHorizontalAlignment( TextHAlignment::LEFT );
    m_nameEditBox->setPosition( m_nameEditBoxHidePos );
    m_nameEditBox->setInputMode( EditBox::InputMode::SINGLE_LINE );
    m_nameEditBox->setReturnType(EditBox::KeyboardReturnType::DONE);
    m_nameEditBox->setText( m_loginUser.userName.c_str() );

    this->addChild( m_nameEditBox );
    
    
    auto t_birthdayLabel = Label::createWithTTF( "生日:", PAGE_FONT, 16 );
    auto t_birthdayLabelSizeHalf = t_birthdayLabel->getContentSize() * 0.5f * fontScale;
    
    auto t_birthdayEditBackground = TexturePacker::Dialog::createTexteditBackgroundSprite();
    auto t_birthdayEditBackgroundSizeHalf = t_birthdayEditBackground->getContentSize() * 0.5f;
    
    
    t_birthdayLabel->setScale( fontScale );
    t_birthdayLabel->setTextColor( Color4B( 0xFF, 0xFF, 0xFF, 0xFF ) );
    t_birthdayLabel->enableOutline( Color4B( 0xC6, 0x72, 0x0C, 0xFF ), 2 );
    
    t_birthdayLabel->setPosition( Vec2( t_listPosX + t_birthdayLabelSizeHalf.width, t_DialogCentBackgroundSize.height - t_padding * 1.8f - t_birthdayEditBackgroundSizeHalf.height - t_nameEditBackgroundSizeHalf.height * 2.0f ) );
    m_dialogCentBackground->addChild( t_birthdayLabel );
    
    t_birthdayEditBackground->setPosition( Vec2( t_listPosX + t_birthdayLabelSizeHalf.width * 2.0f + t_padding * 0.6f + t_birthdayEditBackgroundSizeHalf.width, t_DialogCentBackgroundSize.height - t_padding * 1.8f - t_nameEditBackgroundSizeHalf.height - t_nameEditBackgroundSizeHalf.height * 2.0f ) );
    m_dialogCentBackground->addChild( t_birthdayEditBackground );
    
    
    float t_sexPosY = t_DialogCentBackgroundSize.height - t_padding * 2.8f - t_nameEditBackgroundSizeHalf.height * 2.0f - t_nameEditBackgroundSizeHalf.height * 2.0f;
    
    auto t_girlRedioButton = TexturePacker::Dialog::createRedioButtonNormalSprite();
    auto t_girlRedioButtonSizeHalf = t_girlRedioButton->getContentSize() * 0.5f;
    
    
    auto t_girlLabel = Label::createWithTTF( "女生", PAGE_FONT, 16 );
    t_girlLabel->setScale( fontScale );
    t_girlLabel->setTextColor( Color4B( 0xC6, 0x72, 0x0C, 0xFF ) );
    auto t_girlLabelSizeHalf = t_girlLabel->getContentSize() * 0.5f;
    
    auto t_redioButtonGirl = Button::create( "Empty.png" );
    t_redioButtonGirl->setScale9Enabled( true );
    
    
    Size t_redioButtonGirlSize = Size( ( t_girlRedioButtonSizeHalf.width + t_girlLabelSizeHalf.width ) * 2.0f + t_padding * 0.8f, MAX( t_girlRedioButtonSizeHalf.height, t_girlLabelSizeHalf.height ) * 2.0f );
    t_redioButtonGirl->setContentSize( t_redioButtonGirlSize );
    t_redioButtonGirl->setScale9Enabled( true );
    t_redioButtonGirl->setPosition( Vec2( t_listPosX + t_redioButtonGirlSize.width * 0.5f, t_sexPosY ) );
    m_dialogCentBackground->addChild( t_redioButtonGirl );
    
    t_girlRedioButton->setPosition( Vec2( t_girlRedioButtonSizeHalf.width, t_redioButtonGirlSize.height * 0.5f ) );
    t_redioButtonGirl->addChild( t_girlRedioButton );
    
    t_girlLabel->setPosition( Vec2( t_girlRedioButtonSizeHalf.width * 2.0f + t_padding * 0.4f + t_girlLabelSizeHalf.width, t_redioButtonGirlSize.height * 0.5f  ) );
    t_redioButtonGirl->addChild( t_girlLabel );
    
    
    float t_boyPosX = t_listPosX + t_redioButtonGirlSize.width + t_padding ;
    
    auto t_boyRedioButton = TexturePacker::Dialog::createRedioButtonNormalSprite();
    auto t_boyRedioButtonSizeHalf = t_boyRedioButton->getContentSize() * 0.5f;
    
    auto t_boyLabel = Label::createWithTTF( "男生", PAGE_FONT, 16 );
    auto t_boyLabelSizeHalf = t_boyLabel->getContentSize() * 0.5f;
    t_boyLabel->setScale( fontScale );
    t_boyLabel->setTextColor( Color4B( 0xC6, 0x72, 0x0C, 0xFF ) );
    
    auto t_redioButtonBoy = Button::create( "Empty.png" );
    Size t_redioButtonBoySize = Size( ( t_boyRedioButtonSizeHalf.width + t_boyLabelSizeHalf.width ) * 2.0f + t_padding * 0.8f, MAX( t_boyRedioButtonSizeHalf.height, t_boyLabelSizeHalf.height ) * 2.0f );
    t_redioButtonBoy->setContentSize( t_redioButtonBoySize );
    t_redioButtonBoy->setScale9Enabled( true );
    t_redioButtonBoy->setPosition( Vec2( t_boyPosX + t_redioButtonBoySize.width * 0.5f, t_sexPosY ) );
    m_dialogCentBackground->addChild( t_redioButtonBoy );
    
    t_boyRedioButton->setPosition( Vec2( t_boyRedioButtonSizeHalf.width, t_redioButtonBoySize.height * 0.5f ) );
    t_redioButtonBoy->addChild( t_boyRedioButton );
    
    
    t_boyLabel->setPosition( Vec2( t_boyRedioButtonSizeHalf.width * 2.0f + t_padding * 0.4f + t_boyLabelSizeHalf.width , t_redioButtonBoySize.height * 0.5f ) );
    t_redioButtonBoy->addChild( t_boyLabel );
    
    touchAnswer( t_redioButtonGirl, [this, t_girlRedioButton, t_boyRedioButton]( Ref * p_ref ){
        m_loginUser.userSex = 2;
        refreshSexRedio( t_girlRedioButton, t_boyRedioButton );
    });
    
    touchAnswer( t_redioButtonBoy , [this, t_girlRedioButton, t_boyRedioButton]( Ref * p_ref ){
        m_loginUser.userSex = 1;
        refreshSexRedio( t_girlRedioButton, t_boyRedioButton );
    });
    
    refreshSexRedio( t_girlRedioButton, t_boyRedioButton );
    
    auto t_personalContent = TexturePacker::Dialog::createSettingsContentBackgroundSprite();
    auto t_personalContentSizeHelf = t_personalContent->getContentSize() * 0.5f;

    t_personalContent->setPosition( Vec2( t_DialogCentBackgroundSize.width * 0.5f, t_personalContentSizeHelf.height + t_DialogCentBackgroundSize.width * 0.5f - t_personalContentSizeHelf.width ) );

    m_dialogCentBackground->addChild( t_personalContent );
    
    for( int i = 1; i < 3; ++i )
    {
        auto spaceLine = TexturePacker::Dialog::createListSpaceLineSprite();
        spaceLine->setPosition( Vec2( t_personalContentSizeHelf.width, t_personalContentSizeHelf.height / 1.5f * i ) );
        t_personalContent->addChild( spaceLine );
    }
    
    auto t_itemSize = Size( t_personalContentSizeHelf.width * 2.0f, t_personalContentSizeHelf.height / 1.5f );
    
    float t_tiemPadding = t_itemSize.width * 0.06f;
    
    auto t_account = Button::create( "Empty.png" );
    t_account->setScale9Enabled( true );
    t_account->setContentSize( t_itemSize );
    t_account->setPosition( Vec2( t_personalContentSizeHelf.width, t_personalContentSizeHelf.height + t_itemSize.height ) );
    
    t_personalContent->addChild( t_account );
    
    auto t_accountLabel = Label::createWithTTF( "账户", PAGE_FONT, 12 );
    auto t_accountLabelSizeHalf = t_accountLabel->getContentSize() * 0.5f;
    
    t_accountLabel->setScale( fontScale );
    t_accountLabel->setTextColor( Color4B( 0xC6, 0x72, 0x0C, 0xFF ) );
    t_accountLabel->setPosition( Vec2( t_accountLabelSizeHalf.width + t_tiemPadding, t_itemSize.height * 0.5f ) );
    
    t_account->addChild( t_accountLabel );
    
    auto t_accountType = TexturePacker::Dialog::createLoginByWeChatSprite();

    switch( m_loginUser.loginType )
    {
        case DataUser::LoginType::phone:
            t_accountType->setSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( TexturePacker::Dialog::loginByPhone ) );
        break;
        case DataUser::LoginType::wechat:
            t_accountType->setSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( TexturePacker::Dialog::loginByWeChat ) );
        break;
        case DataUser::LoginType::sina:
            t_accountType->setSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( TexturePacker::Dialog::loginByXinlang ) );
        break;
    }

    auto t_accountTypeSizeHalf = t_accountType->getContentSize() * 0.5f;
    
    t_accountType->setPosition( Vec2( t_itemSize.width - t_accountTypeSizeHalf.width - t_tiemPadding, t_itemSize.height * 0.5f ) );
    t_account->addChild( t_accountType );
    
    auto t_cache = Button::create( "Empty.png" );
    t_cache->setScale9Enabled( true );
    t_cache->setContentSize( t_itemSize );
    t_cache->setPosition( Vec2( t_personalContentSizeHelf.width, t_personalContentSizeHelf.height ) );
    
    t_personalContent->addChild( t_cache );
    
    auto t_cacheLabel = Label::createWithTTF( "缓存", PAGE_FONT, 12 );
    auto t_cacheLabelSizeHalf = t_cacheLabel->getContentSize() * 0.5f;
    
    t_cacheLabel->setScale( fontScale );
    t_cacheLabel->setTextColor( Color4B( 0xC6, 0x72, 0x0C, 0xFF ) );
    t_cacheLabel->setPosition( Vec2( t_cacheLabelSizeHalf.width + t_tiemPadding, t_itemSize.height * 0.5f ) );
    
    t_cache->addChild( t_cacheLabel );
    
    auto t_cacheArrowLabel = Label::createWithTTF( ">", PAGE_FONT, 12 );
    auto t_cacheArrowLabelSizeHalf = t_cacheArrowLabel->getContentSize() * 0.5f;
    
    t_cacheArrowLabel->setScale( fontScale );
    t_cacheArrowLabel->setTextColor( Color4B( 0xC6, 0x72, 0x0C, 0xFF ) );
    t_cacheArrowLabel->setPosition( Vec2( t_itemSize.width - t_cacheArrowLabelSizeHalf.width - t_tiemPadding, t_itemSize.height * 0.5f ) );
    
    t_cache->addChild( t_cacheArrowLabel );
    
    auto t_logout = Button::create( "Empty.png" );
    
    t_logout->setScale9Enabled( true );
    t_logout->setContentSize( t_itemSize );
    t_logout->setPosition( Vec2( t_personalContentSizeHelf.width, t_personalContentSizeHelf.height - t_itemSize.height ) );
    
    t_personalContent->addChild( t_logout );
    
    auto t_logoutLabel = Label::createWithTTF( "注销", PAGE_FONT, 12 );
    auto t_logoutLabelSizeHalf = t_logoutLabel->getContentSize() * 0.5f;
    
    t_logoutLabel->setScale( fontScale );
    t_logoutLabel->setTextColor( Color4B( 0xC6, 0x72, 0x0C, 0xFF ) );
    t_logoutLabel->setPosition( Vec2( t_logoutLabelSizeHalf.width + t_tiemPadding, t_itemSize.height * 0.5f ) );
    
    t_logout->addChild( t_logoutLabel );


    //注销登陆
    touchAnswer( t_logout, []( Ref * p_ref ){
        DataTableUser::instance().logout();

        auto t_loginUser = DataTableUser::instance().getActivation();

        Http::Post( CONFIG_GOOFYPAPA_DOMAIN + "/user/auth/logout.do", nullptr, []( std::string p_res ){

        },[]( std::string p_res ){

        } );
        Http::token = "";
        Director::getInstance()->replaceScene( LoginScene::create() );
    }, 1.05f );

    
    return true;
}

void DialogPersonalCenterLayer::show()
{
    clearAllActions();
    
    m_vague->setOpacity( 0.0f );
    m_vague->setVisible( true );
    m_vague->runAction( ActionFloat::create( 0.2f, 0.0f, 1.0f, [this]( float p_data ){
        m_vague->setOpacity( p_data * 255 );
    } ) );
    
    m_dialogCentBackground->setVisible(true);
    m_dialogCentSaveButton->setVisible(true);
    
    m_dialogCentBackground->setPosition( m_dialogCentBackgroundShowPos + Vec2( 0.0f, 50.0f ) );
    m_dialogCentBackground->runAction( MoveTo::create( 0.2f, m_dialogCentBackgroundShowPos ) );
    
    
    m_dialogCentSaveButton->setPosition( m_dialogCentSaveButtonShowPos - Vec2( 0.0f, 50.0f ) );
    m_dialogCentSaveButton->runAction( MoveTo::create( 0.2f, m_dialogCentSaveButtonShowPos ) );
    
    m_nameEditBox->runAction( MoveTo::create( 0.2f, m_nameEditBoxShowPos ) );
}

void DialogPersonalCenterLayer::hide()
{
    
    clearAllActions();
    
    m_vague->runAction( ActionFloat::create( 0.1f, 1.0f, 0.0f, [this]( float p_data )
                                            {
                                                m_vague->setOpacity( p_data * 255 );
                                                if( p_data >= 0.2f && m_vague->isVisible() )
                                                {
                                                    m_vague->setVisible( false );
                                                    m_dialogCentBackground->setVisible(false);
                                                    m_dialogCentSaveButton->setVisible(false);
                                                }
                                            } ) );
    m_dialogCentBackground->runAction( MoveTo::create( 0.2f, m_dialogCentBackgroundHiedPos ) );
    m_dialogCentSaveButton->runAction( MoveTo::create( 0.2f, m_dialogCentSaveButtonHidePos ) );
    m_nameEditBox->runAction( MoveTo::create( 0.2f, m_nameEditBoxHidePos ) );

    auto t_oldUserInfo = DataTableUser::instance().getActivation();

    m_loginUser.userName = m_nameEditBox->getText();

    if( t_oldUserInfo.userName != m_loginUser.userName || t_oldUserInfo.userSex != m_loginUser.userSex || t_oldUserInfo.userBirthday != m_loginUser.userBirthday )
    {
        std::map< std::string, std::string > t_parameter;
        t_parameter["userId"] = m_loginUser.userId;
        t_parameter["userName"] = m_loginUser.userName;
        t_parameter["userMobile"] = "";
        t_parameter["userEmail"] = "";
        
        Http::Post( CONFIG_GOOFYPAPA_DOMAIN + "/user/update.do", &t_parameter, []( std::string p_str ){
            printf( "/user/update.do success: %s \n", p_str.c_str() );
        }, []( std::string p_str ){
            printf( "/user/update.do final: %s \n", p_str.c_str() );
        } );

        DataTableUser::instance().update( m_loginUser );
    }
    
    if( hideCallBack )
    {
        hideCallBack();
    }
}

void DialogPersonalCenterLayer::clearAllActions()
{
    m_vague->stopAllActions();
    m_vague->getActionManager()->removeAllActions();
    
    m_dialogCentBackground->stopAllActions();
    m_dialogCentBackground->getActionManager()->removeAllActions();
    
    m_dialogCentSaveButton->stopAllActions();
    m_dialogCentSaveButton->getActionManager()->removeAllActions();
}

void DialogPersonalCenterLayer::refreshSexRedio( Ref * p_girlRedio, Ref * p_boyRedio )
{
    auto girlRedioSprite = ( Sprite* )p_girlRedio;
    auto boyRedioSprite = ( Sprite* )p_boyRedio;
    if( m_loginUser.userSex == 2 )
    {
        girlRedioSprite->setSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( TexturePacker::Dialog::redioButtonSelectedOrange ) );
        boyRedioSprite->setSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( TexturePacker::Dialog::redioButtonNormal ) );
    }else if( m_loginUser.userSex == 1 )
    {
        girlRedioSprite->setSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( TexturePacker::Dialog::redioButtonNormal ) );
        boyRedioSprite->setSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( TexturePacker::Dialog::redioButtonSelectedBlue ) );
    }
}
