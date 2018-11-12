//
//  CadenceGameMusicListScrollView.cpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/11/8.
//

#include "CadenceGameMusicListScrollView.h"

USING_NS_CC;

bool CadenceGameMusicListScrollView::init( void )
{
    if( !cocos2d::ui::ScrollView::init() )
    {
        return false;
    }
    
    return true;
}

bool CadenceGameMusicListScrollView::initWithData( const std::vector< std::pair< std::string, std::string > > & p_dataList )
{
    auto t_contentSize = getContentSize();
    for( auto i = 0; i < p_dataList.size(); ++i )
    {
        auto t_label = Label::createWithSystemFont( p_dataList[i].second , "", 20 );
        t_label->setPosition( Vec2( t_contentSize.width * 0.5f, ( p_dataList.size() - i - 0.5f ) * 30.0f  ) );
        addChild( t_label );
    }
    
    return true;
}

bool CadenceGameMusicListScrollView::onTouchBegan( cocos2d::Touch * p_touch, cocos2d::Event * p_unusedEvent)
{
    if( !ScrollView::onTouchBegan( p_touch, p_unusedEvent ) )
    {
        return false;
    }
    
    m_touchBeginLocationList[ p_touch->getID() ] = p_touch->getLocation();
    
    return true;
}

void CadenceGameMusicListScrollView::onTouchMoved( cocos2d::Touch * p_touch, cocos2d::Event * p_unusedEvent)
{
    ScrollView::onTouchMoved( p_touch, p_unusedEvent );
}
void CadenceGameMusicListScrollView::onTouchEnded( cocos2d::Touch * p_touch, cocos2d::Event * p_unusedEvent)
{
    ScrollView::onTouchEnded( p_touch, p_unusedEvent );
    
    auto t_beginPos = m_touchBeginLocationList[p_touch->getID()];
    m_touchBeginLocationList.erase( p_touch->getID() );
    
    if( t_beginPos.distance( p_touch->getLocation() ) < 2.0f )
    {
        auto t_coutentHieght = getContentSize().height;
        auto t_postion = convertToNodeSpace( t_beginPos );
        auto t_offsetV = ( getScrolledPercentVertical() / 100.0f ) * ( getInnerContainerSize().height - t_coutentHieght ) ;
        auto t_selectIndex = (int)floor( ( t_offsetV + t_coutentHieght - t_postion.y ) / 30.0f );
        
        onTouched( t_selectIndex );
    }
}
