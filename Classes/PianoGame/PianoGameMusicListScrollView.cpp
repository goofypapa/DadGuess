//
//  PianoGameMusicListScrollView.cpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/10/31.
//

#include "PianoGameMusicListScrollView.h"
#include "PianoGame.hpp"
#include <string>


USING_NS_CC;
using namespace cocos2d::ui;


std::vector< std::pair< std::string, std::string > > PianoGameMusicListScrollView::sm_musicList;

PianoGameMusicListScrollView * PianoGameMusicListScrollView::create( const cocos2d::Size & p_contentSize, const float p_itemHeight )
{
    auto t_result = create();
    
    if( !t_result->initWithContentSize( p_contentSize, p_itemHeight ) )
    {
        return nullptr;
    }
    
    return t_result;
}

bool PianoGameMusicListScrollView::init( void )
{
    if( !ScrollView::init() )
    {
        return false;
    }
    selectIndex = 0;
    return true;
}

bool PianoGameMusicListScrollView::initWithContentSize( const cocos2d::Size & p_contentSize, const float p_itemHeight )
{
    m_itemHeight = p_itemHeight;
    
    
    int t_itemCount = (int)sm_musicList.size();
    
    setContentSize( p_contentSize );
    setInnerContainerSize( Size( p_contentSize.width, p_itemHeight * (float)t_itemCount ) );
    
    setDirection( ScrollView::Direction::VERTICAL );
    
    
    m_selectedSprite = TexturePacker::PianoGame::createMusicListSelectedSprite();
    auto t_selectedSpriteSize = m_selectedSprite->getContentSize();
    m_selectedSprite->setScale( p_contentSize.width / t_selectedSpriteSize.width, p_itemHeight / t_selectedSpriteSize.height * 1.4f );
    m_selectedSprite->setPosition( Vec2(  p_contentSize.width * 0.5f, p_itemHeight * ( (float)(t_itemCount - selectIndex) - 0.5f ) ) );
    addChild( m_selectedSprite );
    
    for( int i = 0; i < t_itemCount; ++i )
    {
        auto t_item = Label::createWithSystemFont( sm_musicList[i].second.c_str() , "", 14 );
        
        t_item->setPosition( Vec2( p_contentSize.width * 0.5f, p_itemHeight * ( (float)(t_itemCount - i) - 0.5f ) ) );
        
        addChild( t_item );
    }
    
    return true;
}

bool PianoGameMusicListScrollView::onTouchBegan( cocos2d::Touch * p_touch, cocos2d::Event * p_unusedEvent)
{
    if( !ScrollView::onTouchBegan( p_touch, p_unusedEvent ) )
    {
        return false;
    }
    
    m_touchBeginLocationList[ p_touch->getID() ] = p_touch->getLocation();
    
    return true;
}

void PianoGameMusicListScrollView::onTouchMoved( cocos2d::Touch * p_touch, cocos2d::Event * p_unusedEvent)
{
    ScrollView::onTouchMoved( p_touch, p_unusedEvent );
}
void PianoGameMusicListScrollView::onTouchEnded( cocos2d::Touch * p_touch, cocos2d::Event * p_unusedEvent)
{
    ScrollView::onTouchEnded( p_touch, p_unusedEvent );
    
    auto t_beginPos = m_touchBeginLocationList[p_touch->getID()];
    m_touchBeginLocationList.erase( p_touch->getID() );
    
    if( t_beginPos.distance( p_touch->getLocation() ) < 2.0f )
    {
        auto t_coutentHieght = getContentSize().height;
        auto t_postion = convertToNodeSpace( t_beginPos );
        auto t_offsetV = ( getScrolledPercentVertical() / 100.0f ) * ( getInnerContainerSize().height - t_coutentHieght ) ;
        selectIndex = (int)floor( ( t_offsetV + t_coutentHieght - t_postion.y ) / m_itemHeight );
        
        m_selectedSprite->stopAllActions();
        m_selectedSprite->runAction( MoveTo::create( 0.2f , Vec2( getContentSize().width * 0.5f, getInnerContainerSize().height - m_itemHeight * ( selectIndex + 0.5f ) ) ) );
    }
}
void PianoGameMusicListScrollView::onTouchCancelled( cocos2d::Touch * p_touch, cocos2d::Event * p_unusedEvent)
{
    ScrollView::onTouchCancelled( p_touch, p_unusedEvent );
}
