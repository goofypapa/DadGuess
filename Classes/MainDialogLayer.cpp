#include "MainDialogLayer.h"
#include "Resources/Dialog.hpp"
#include "Common.h"

USING_NS_CC;
using namespace cocos2d::ui;

bool MainDialogLayer::init()
{
    if( !Layer::init() )
    {
        return false;
    }

    hideCallBack = nullptr;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    m_vague = DrawNode::create();
    m_vague->setPosition( -Vec2( visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y ) );
    m_vague->drawSolidRect( origin, origin + visibleSize, Color4F( 0.0f, 0.0f, 0.0f, 0.6f ) );
    m_vague->setVisible( false );
    this->addChild( m_vague );

    m_dialogCentBackground = TexturePacker::Dialog::createSettingBackgroundSprite();
    auto t_DialogCentBackgroundSize = m_dialogCentBackground->getContentSize();
    this->addChild(m_dialogCentBackground);

    m_dialogCentSaveButton = Button::create( TexturePacker::Dialog::saveButton, TexturePacker::Dialog::saveButton, "", TextureResType::PLIST );
    auto t_DialogCentSaveButtonSize = m_dialogCentSaveButton->getContentSize();

    m_dialogCentSaveButton->setScale( adaptation() );

    touchAnswer( m_dialogCentSaveButton, [this]( Ref * p_ref ){
        hide();
    } );

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

    return true;
}

void MainDialogLayer::show()
{
    clearAllActions();
    
    m_vague->setOpacity( 0.0f );
    m_vague->setVisible( true );
    m_vague->runAction( ActionFloat::create( 0.2f, 0.0f, 1.0f, [this]( float p_data ){
        m_vague->setOpacity( p_data * 255 );
    } ) );

    m_dialogCentBackground->setPosition( m_dialogCentBackgroundShowPos + Vec2( 0.0f, 50.0f ) );
    m_dialogCentBackground->setOpacity( 0 );
    m_dialogCentBackground->runAction( MoveTo::create( 0.2f, m_dialogCentBackgroundShowPos ) );
    m_dialogCentBackground->runAction( ActionFloat::create( 0.2f, 0.0f, 1.0f, [this]( float p_data ){
        m_dialogCentBackground->setOpacity( p_data * 255 );
    } ) );

    m_dialogCentSaveButton->setPosition( m_dialogCentSaveButtonShowPos - Vec2( 0.0f, 50.0f ) );
    m_dialogCentSaveButton->setOpacity( 0 );
    m_dialogCentSaveButton->runAction( MoveTo::create( 0.2f, m_dialogCentSaveButtonShowPos ) );
    m_dialogCentSaveButton->runAction( ActionFloat::create( 0.2f, 0.0f, 1.0f, [this]( float p_data ){
        m_dialogCentSaveButton->setOpacity( p_data * 255 );
    } ) );
}

void MainDialogLayer::hide()
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

void MainDialogLayer::clearAllActions()
{
    m_vague->stopAllActions();
    m_vague->getActionManager()->removeAllActions();

    m_dialogCentBackground->stopAllActions();
    m_dialogCentBackground->getActionManager()->removeAllActions();

    m_dialogCentSaveButton->stopAllActions();
    m_dialogCentSaveButton->getActionManager()->removeAllActions();
}
