#include "Message.h"

USING_NS_CC;

#define PAGE_FONT "fonts/HuaKangFangYuanTIW7-GB_0.ttf"

bool Message::init( void )
{
    if( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    m_messageBoxBG = ui::Scale9Sprite::create( "MessageBoxBG.png" );
    if( m_messageBoxBG != nullptr )
    {
        m_messageBoxBG->setPosition( Vec2( visibleSize.width * 0.5f + origin.x, visibleSize.height * 0.5f + origin.y ) );
        this->addChild( m_messageBoxBG );
    }
    
    m_messageLabel = Label::createWithTTF( "哈哈1234", PAGE_FONT, 12 );
    if( m_messageLabel != nullptr )
    {
        Size t_boxSize = m_messageLabel->getContentSize() + Size( 10.0f, 10.0f );
        m_messageLabel->setPosition( Vec2( t_boxSize.width * 0.5f, t_boxSize.height * 0.5f ) );
        m_messageBoxBG->addChild( m_messageLabel );
        m_messageBoxBG->setPreferredSize( t_boxSize );
    }
    
    return true;
}

void Message::show( const std::string & p_str )
{
    if( m_messageLabel != nullptr )
    {
        m_messageLabel->setString( p_str );
        m_messageBoxBG->setPreferredSize( m_messageLabel->getContentSize() );
    }
}
