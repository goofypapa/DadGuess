#include "MainPersonalLayer.h"
#include "Resources/Personal.hpp"
#include "Common.h"

USING_NS_CC;
using namespace cocos2d::ui;

bool MainPersonalLayer::init()
{
    if( !Layer::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    m_vague = DrawNode::create();
    m_vague->setPosition( -Vec2( visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y ) );
    m_vague->drawSolidRect( origin, origin + visibleSize, Color4F( 0.0f, 0.0f, 0.0f, 0.6f ) );
    m_vague->setVisible( false );
    this->addChild( m_vague );

    m_personalCentBackground = TexturePacker::Personal::createSettingBackgroundSprite();
    auto t_personalCentBackgroundSize = m_personalCentBackground->getContentSize();
    this->addChild(m_personalCentBackground);

    m_personalContSaveButton = Button::create( TexturePacker::Personal::saveButton, TexturePacker::Personal::saveButton, "", TextureResType::PLIST );
    auto t_personalCentSaveButtonSize = m_personalContSaveButton->getContentSize();

    m_personalContSaveButton->setScale( adaptation() );

    float t_personalCentSaveButtonHeight = t_personalCentSaveButtonSize.height * adaptation();
    this->setPosition( Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y + t_personalCentSaveButtonHeight * 0.5f ) );
    float t_personalCentBackgroundHeight = visibleSize.height - ( t_personalCentSaveButtonHeight * 1.6f ) * 2.0f;

    m_personalCentBackgroundHiedPos = Vec2( 0.0f, visibleSize.height * 0.5f + t_personalCentBackgroundHeight * 0.5f );
    m_personalCentBackgroundShowPos = Vec2( 0.0f, t_personalCentSaveButtonHeight * 0.5f );

    m_personalContSaveButtonHidePos = Vec2( 0.0f, -(visibleSize.height * 0.5f + t_personalCentSaveButtonHeight * 0.5f) );
    m_personalContSaveButtonShowPos = Vec2( 0.0f, -(visibleSize.height * 0.5f - t_personalCentSaveButtonHeight * 1.3f) );
    
    m_personalCentBackground->setPosition( m_personalCentBackgroundHiedPos );
    m_personalContSaveButton->setPosition( m_personalContSaveButtonHidePos );
    this->addChild( m_personalContSaveButton );
    
    m_personalCentBackground->setScale( t_personalCentBackgroundHeight / t_personalCentBackgroundSize.height );

    return true;
}

void MainPersonalLayer::show()
{
    m_vague->setOpacity( 0.0f );
    m_vague->setVisible( true );
    m_vague->runAction( ActionFloat::create( 0.1f, 0.0f, 1.0f, [this]( float p_data ){
        m_vague->setOpacity( p_data * 255 );
    } ) );

    m_personalCentBackground->setPosition( m_personalCentBackgroundShowPos + Vec2( 0.0f, 50.0f ) );
    m_personalCentBackground->setOpacity( 0 );
    m_personalCentBackground->runAction( MoveTo::create( 0.2f, m_personalCentBackgroundShowPos ) );
    m_personalCentBackground->runAction( ActionFloat::create( 0.2f, 0.0f, 1.0f, [this]( float p_data ){
        m_personalCentBackground->setOpacity( p_data * 255 );
    } ) );

    m_personalContSaveButton->setPosition( m_personalContSaveButtonShowPos - Vec2( 0.0f, 50.0f ) );
    m_personalContSaveButton->setOpacity( 0 );
    m_personalContSaveButton->runAction( MoveTo::create( 0.2f, m_personalContSaveButtonShowPos ) );
    m_personalContSaveButton->runAction( ActionFloat::create( 0.2f, 0.0f, 1.0f, [this]( float p_data ){
        m_personalContSaveButton->setOpacity( p_data * 255 );
    } ) );
}

void MainPersonalLayer::hide()
{
    m_vague->runAction( ActionFloat::create( 0.1f, 1.0f, 0.0f, [this]( float p_data )
    {
        m_vague->setOpacity( p_data * 255 );
        if( p_data == 0.2f )
        {
            m_vague->setVisible( false );
        }
    } ) );
    m_personalCentBackground->runAction( MoveTo::create( 0.2f, m_personalCentBackgroundHiedPos ) );
    m_personalContSaveButton->runAction( MoveTo::create( 0.2f, m_personalContSaveButtonHidePos ) );
}