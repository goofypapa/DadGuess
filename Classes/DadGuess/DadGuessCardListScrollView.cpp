//
//  DadGuessCardListScrollView.cpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/11/22.
//

#include "DadGuessCardListScrollView.h"
#include "DadGuessUpdateScene.h"
#include "WebViewScene.h"
#include "Common.h"

USING_NS_CC;
using namespace cocos2d::ui;

int DadGuessCardListScrollView::sm_columns = 4;

DadGuessCardListScrollView * DadGuessCardListScrollView::createWithSize( const cocos2d::Size p_size, const std::string p_groupId )
{
    auto t_result = create();
    
    if( !t_result->initWithSize( p_size, p_groupId ) )
    {
        t_result = nullptr;
    }
    
    return t_result;
}

bool DadGuessCardListScrollView::init( void )
{
    if( !ScrollView::init() )
    {
        return false;
    }
    
    return true;
}

bool DadGuessCardListScrollView::initWithSize( const cocos2d::Size & p_contentSize, const std::string p_groupId )
{
    setContentSize( p_contentSize );
    
    m_groupId = p_groupId;
    auto & t_cardList = DadGuessUpdateScene::s_cardList[m_groupId];
    
    int t_rowCount = (int)ceil(t_cardList.size() / sm_columns);
    float t_itemSize = p_contentSize.width / sm_columns;
    float t_itemPadding = 2.0f;
    
    float t_innerContainerHeight = t_rowCount * t_itemSize;
    
    setInnerContainerSize( Size( p_contentSize.width, t_innerContainerHeight ) );
    
    setDirection( ScrollView::Direction::VERTICAL );
    
    for( int i = 0; i < t_cardList.size(); ++i )
    {
        
        auto t_itemTexture = Director::getInstance()->getTextureCache()->getTextureForKey( t_cardList[i].second.fileName );
        
        if( !t_itemTexture )
        {
            printf( "---------->: %s \n", t_cardList[i].first.toJson().c_str() );
            continue;
        }
        
        auto t_itemSprite = Sprite::createWithTexture( t_itemTexture );
        
        auto t_itemSpriteSize = t_itemSprite->getContentSize();
        
        t_itemSprite->setScale( MIN( ( t_itemSize - t_itemPadding ) / t_itemSpriteSize.width, ( t_itemSize - t_itemPadding ) / t_itemSpriteSize.height ) );
        
        int t_row = (int)floor( i / sm_columns );
        int t_column = i % sm_columns;

        t_itemSprite->setPosition( Vec2( t_itemSize * ( t_column + 0.5f ), t_innerContainerHeight - t_itemSize * ( t_row + 0.5f ) ) );

        addChild( t_itemSprite );
        
    }
    
    return true;
}

void DadGuessCardListScrollView::onTouched( const int p_index )
{
    auto & t_cardList = DadGuessUpdateScene::s_cardList[m_groupId];

    if( m_groupId.compare( DataCardBatchInfo::s_batchIdList[0] ) == 0 )
    {
        setAppOrientation( true );
    }

    Director::getInstance()->replaceScene( WebViewScene::createWithDir( m_groupId, m_groupId.compare( DataCardBatchInfo::s_batchIdList[0] ) == 0, t_cardList[p_index].first.id ) );
}

bool DadGuessCardListScrollView::onTouchBegan( cocos2d::Touch * p_touch, cocos2d::Event * p_unusedEvent)
{
    if( !ScrollView::onTouchBegan( p_touch, p_unusedEvent ) )
    {
        return false;
    }
    
    m_touchBeginLocationList[ p_touch->getID() ] = p_touch->getLocation();
    
    return true;
}

void DadGuessCardListScrollView::onTouchMoved( cocos2d::Touch * p_touch, cocos2d::Event * p_unusedEvent)
{
    ScrollView::onTouchMoved( p_touch, p_unusedEvent );
}
void DadGuessCardListScrollView::onTouchEnded( cocos2d::Touch * p_touch, cocos2d::Event * p_unusedEvent)
{
    ScrollView::onTouchEnded( p_touch, p_unusedEvent );
    
    auto t_beginPos = m_touchBeginLocationList[p_touch->getID()];
    m_touchBeginLocationList.erase( p_touch->getID() );
    
    if( t_beginPos.distance( p_touch->getLocation() ) < 2.0f )
    {
        auto t_coutentHieght = getContentSize().height;
        auto t_postion = convertToNodeSpace( t_beginPos );
        auto t_offsetV = ( getScrolledPercentVertical() / 100.0f ) * ( getInnerContainerSize().height - t_coutentHieght ) ;
        
        auto t_realPos = Vec2( t_postion.x, getContentSize().height - t_postion.y + t_offsetV );
        
        auto t_contentSize = getContentSize();
        float t_itemWidthHeight = t_contentSize.width / sm_columns;
        
        int t_row = (int)floor( t_realPos.y / t_itemWidthHeight );
        int t_column = (int)floor( t_realPos.x / t_itemWidthHeight );
        
        auto t_selectIndex = t_row * sm_columns + t_column;
        
        onTouched( t_selectIndex );
    }
}
