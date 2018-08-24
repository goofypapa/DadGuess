//
//  DialogSettingsLayer.cpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/8/8.
//

#include "DialogSettingsLayer.h"
#include "Dialog.hpp"
#include "Common.h"

USING_NS_CC;
using namespace cocos2d::ui;

#define PAGE_FONT "fonts/HuaKangFangYuanTIW7-GB_0.ttf"

bool DialogSettingsLayer::init()
{
    if( !Layer::init() )
    {
        return false;
    }
    
    m_enableBGM = false;
    
    hideCallBack = nullptr;
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    float fontScale = 1.0f / adaptation();
    
    m_vague = DrawNode::create();
    m_vague->setPosition( -Vec2( visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y ) );
    m_vague->drawSolidRect( origin, origin + visibleSize, Color4F( 0.0f, 0.0f, 0.0f, 0.6f ) );
    m_vague->setVisible( false );
    this->addChild( m_vague );
    
    m_dialogCentBackground = TexturePacker::Dialog::createSettingBackgroundSprite();
    auto t_DialogCentBackgroundSize = m_dialogCentBackground->getContentSize();
    this->addChild(m_dialogCentBackground);
    
    m_dialogCentSaveButton = Button::create( TexturePacker::Dialog::confirmButton, TexturePacker::Dialog::confirmButton, "", TextureResType::PLIST );
    auto t_DialogCentSaveButtonSize = m_dialogCentSaveButton->getContentSize();
    
    m_dialogCentSaveButton->setScale( adaptation() );
    
    touchAnswer( m_dialogCentSaveButton, [this]( Ref * p_ref ){
        hide();
    }, adaptation() * 1.1f, adaptation() );
    
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
    
    
    float t_titleHeight = t_DialogCentBackgroundSize.height * 0.16f;
    
    auto t_title = Label::createWithTTF( "设置", PAGE_FONT, 16 );
    t_title->setScale( fontScale * 0.8f );
    auto t_titleSize = t_title->getContentSize() * fontScale;
    t_title->setPosition( t_DialogCentBackgroundSize.width * 0.5f, t_DialogCentBackgroundSize.height - t_titleHeight + t_titleSize.height * 0.5f );
    t_title->setTextColor( Color4B( 0xFF, 0xFF, 0xFF, 0xFF ) );
    t_title->enableOutline( Color4B( 0xC6, 0x72, 0x0C, 0xFF ), 2 );
    m_dialogCentBackground->addChild( t_title );
    
    auto t_settingContentBorder = TexturePacker::Dialog::createSettingsContentBackgroundSprite();
    auto t_settingContentBorderSize = t_settingContentBorder->getContentSize();
    t_settingContentBorder->setPosition( Vec2( t_DialogCentBackgroundSize.width * 0.5f, (t_DialogCentBackgroundSize.height - t_titleHeight * 0.5f - t_titleSize.height * 0.5f ) * 0.5f ) );
    m_dialogCentBackground->addChild( t_settingContentBorder );
    
    for( int i = 1; i < 3; ++i )
    {
        auto spaceLine = TexturePacker::Dialog::createListSpaceLineSprite();
        spaceLine->setPosition( Vec2( t_settingContentBorderSize.width * 0.5f, t_settingContentBorderSize.height / 3.0f * i ) );
        t_settingContentBorder->addChild( spaceLine );
    }
    
    auto t_itemSize = Size( t_settingContentBorderSize.width, t_settingContentBorderSize.height / 3.0f );
    
    auto t_wifiConnect = Button::create( "Empty.png" );
    t_wifiConnect->setContentSize( t_itemSize );
    t_wifiConnect->setPosition( Vec2( t_settingContentBorderSize.width * 0.5f, t_settingContentBorderSize.height * 2.5f / 3.0f ) );
    t_wifiConnect->setScale9Enabled( true );
    touchAnswer( t_wifiConnect, [this]( Ref * p_ref ){
        
    }, 1.1f );
    t_settingContentBorder->addChild( t_wifiConnect );
    
    float t_tiemPadding = t_settingContentBorderSize.width * 0.06f;
    
    auto t_wifiConnectLabel = Label::createWithTTF( "Wifi连接", PAGE_FONT, 12 );
    t_wifiConnectLabel->setScale( fontScale );
    auto t_wifiConnectLabelSize = t_wifiConnectLabel->getContentSize() * fontScale;
    t_wifiConnectLabel->setPosition( Vec2( t_wifiConnectLabelSize.width * 0.5f + t_tiemPadding, t_settingContentBorderSize.height / 6.0f ) );
    t_wifiConnectLabel->setTextColor( Color4B( 0xC6, 0x72, 0x0C, 0xFF ) );
    t_wifiConnect->addChild( t_wifiConnectLabel );
    
    auto t_wifiConnectArrowLabel = Label::createWithTTF( ">", PAGE_FONT, 12 );
    t_wifiConnectArrowLabel->setScale( fontScale );
    auto t_wifiConnectArrowLabelSize = t_wifiConnectArrowLabel->getContentSize() * fontScale;
    
    t_wifiConnectArrowLabel->setPosition( Vec2( t_settingContentBorderSize.width - t_wifiConnectArrowLabelSize.width * 0.5f - t_tiemPadding, t_settingContentBorderSize.height / 6.0f ) );
    t_wifiConnectArrowLabel->setTextColor( Color4B( 0xC6, 0x72, 0x0C, 0xFF ) );
    t_wifiConnect->addChild( t_wifiConnectArrowLabel );
    
    
    auto t_gameBackgroundMusicSwich = TexturePacker::Dialog::createSwichNormalSprite();
    
    if( m_enableBGM )
    {
        t_gameBackgroundMusicSwich->setSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( TexturePacker::Dialog::swichEnabled ) );
    }
    
    auto t_gameBackgroundMusic = Button::create( "Empty.png" );
    t_gameBackgroundMusic->setContentSize( t_itemSize );
    t_gameBackgroundMusic->setPosition( Vec2( t_settingContentBorderSize.width * 0.5f, t_settingContentBorderSize.height / 2.0f ) );
    t_gameBackgroundMusic->setScale9Enabled( true );
    touchAnswer( t_gameBackgroundMusic, nullptr, nullptr, [this, t_gameBackgroundMusicSwich]( Ref * p_ref ){
        m_enableBGM = !m_enableBGM;
        if( m_enableBGM )
        {
            t_gameBackgroundMusicSwich->setSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( TexturePacker::Dialog::swichEnabled ) );
        }else{
            t_gameBackgroundMusicSwich->setSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( TexturePacker::Dialog::swichNormal ) );
        }
    }, nullptr );
    t_settingContentBorder->addChild( t_gameBackgroundMusic );
    
    auto t_gameBackgroundMusicLabel = Label::createWithTTF( "游戏音效", PAGE_FONT, 12 );
    t_gameBackgroundMusicLabel->setScale( fontScale );
    auto t_gameBackgroundMusicLabelSize = t_gameBackgroundMusicLabel->getContentSize() * fontScale;
    t_gameBackgroundMusicLabel->setPosition( Vec2( t_gameBackgroundMusicLabelSize.width * 0.5f + t_tiemPadding, t_settingContentBorderSize.height / 6.0f ) );
    t_gameBackgroundMusicLabel->setTextColor( Color4B( 0xC6, 0x72, 0x0C, 0xFF ) );
    t_gameBackgroundMusic->addChild( t_gameBackgroundMusicLabel );
    
    
    auto t_gameBackgroundMusicSwichSize = t_gameBackgroundMusicSwich->getContentSize();
    t_gameBackgroundMusicSwich->setPosition( Vec2( t_settingContentBorderSize.width - t_gameBackgroundMusicSwichSize.width * 0.5f - t_tiemPadding, t_settingContentBorderSize.height / 6.0f ) );
    t_gameBackgroundMusic->addChild( t_gameBackgroundMusicSwich );
    
    
    auto t_protocal = Button::create( "Empty.png" );
    t_protocal->setContentSize( t_itemSize );
    t_protocal->setPosition( Vec2( t_settingContentBorderSize.width * 0.5f, t_settingContentBorderSize.height / 6.0f ) );
    t_protocal->setScale9Enabled( true );
    
    touchAnswer( t_protocal, [this]( Ref * p_ref ){
        
    }, 1.1f );
    t_settingContentBorder->addChild( t_protocal );
    
    auto t_protocalLabel = Label::createWithTTF( "用户协议及服务条款", PAGE_FONT, 12 );
    t_protocalLabel->setScale( fontScale );
    auto t_protocalLabelSize = t_protocalLabel->getContentSize() * fontScale;
    
    t_protocalLabel->setPosition( Vec2( t_protocalLabelSize.width * 0.5f + t_tiemPadding, t_settingContentBorderSize.height / 6.0f ) );
    t_protocalLabel->setTextColor( Color4B( 0xC6, 0x72, 0x0C, 0xFF ) );
    t_protocal->addChild( t_protocalLabel );
    
    auto t_protocalArrowLabel = Label::createWithTTF( ">", PAGE_FONT, 12 );
    t_protocalArrowLabel->setScale( fontScale );
    auto t_protocalArrowLabelSize = t_protocalArrowLabel->getContentSize() * fontScale;
    
    t_protocalArrowLabel->setPosition( Vec2( t_settingContentBorderSize.width - t_protocalArrowLabelSize.width * 0.5f - t_tiemPadding, t_settingContentBorderSize.height / 6.0f ) );
    t_protocalArrowLabel->setTextColor( Color4B( 0xC6, 0x72, 0x0C, 0xFF ) );
    t_protocal->addChild( t_protocalArrowLabel );
    
    return true;
}

void DialogSettingsLayer::show()
{
    clearAllActions();
    
    m_vague->setOpacity( 0.0f );
    m_vague->setVisible( true );
    m_vague->runAction( ActionFloat::create( 0.2f, 0.0f, 1.0f, [this]( float p_data ){
        m_vague->setOpacity( p_data * 255 );
    } ) );
    
    m_dialogCentBackground->setPosition( m_dialogCentBackgroundShowPos + Vec2( 0.0f, 50.0f ) );
    m_dialogCentBackground->runAction( MoveTo::create( 0.2f, m_dialogCentBackgroundShowPos ) );
    
    m_dialogCentSaveButton->setPosition( m_dialogCentSaveButtonShowPos - Vec2( 0.0f, 50.0f ) );
    m_dialogCentSaveButton->runAction( MoveTo::create( 0.2f, m_dialogCentSaveButtonShowPos ) );
}

void DialogSettingsLayer::hide()
{
    
    clearAllActions();
    
    m_vague->runAction( ActionFloat::create( 0.1f, 1.0f, 0.0f, [this]( float p_data )
                                            {
                                                m_vague->setOpacity( p_data * 255 );
                                                if( p_data == 0.2f )
                                                {
                                                    m_vague->setVisible( false );
                                                }
                                            } ) );
    m_dialogCentBackground->runAction( MoveTo::create( 0.2f, m_dialogCentBackgroundHiedPos ) );
    m_dialogCentSaveButton->runAction( MoveTo::create( 0.2f, m_dialogCentSaveButtonHidePos ) );
    
    if( hideCallBack )
    {
        hideCallBack();
    }
}

void DialogSettingsLayer::clearAllActions()
{
    m_vague->stopAllActions();
    m_vague->getActionManager()->removeAllActions();
    
    m_dialogCentBackground->stopAllActions();
    m_dialogCentBackground->getActionManager()->removeAllActions();
    
    m_dialogCentSaveButton->stopAllActions();
    m_dialogCentSaveButton->getActionManager()->removeAllActions();
}


void DialogSettingsLayer::wifiConnect( cocos2d::Ref* pSender )
{
    
}
