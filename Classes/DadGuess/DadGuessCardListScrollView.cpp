//
//  DadGuessCardListScrollView.cpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/11/22.
//

#include "DadGuessCardListScrollView.h"
#include "DadGuessUpdateScene.h"

USING_NS_CC;
using namespace cocos2d::ui;

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
    
    int t_columns = 4;

    auto & t_cardList = DadGuessUpdateScene::s_cardList[p_groupId];
    
    int t_rowCount = (int)ceil(t_cardList.size() / t_columns);
    float t_itemSize = p_contentSize.width / t_columns;
    float t_itemPadding = 2.0f;
    
    setInnerContainerSize( Size( p_contentSize.width, t_rowCount * t_itemSize ) );
    
    setDirection( ScrollView::Direction::VERTICAL );
    
    for( int i = 0; i < t_cardList.size(); ++i )
    {
        auto t_itemSprite = Sprite::createWithTexture( Director::getInstance()->getTextureCache()->getTextureForKey( t_cardList[i].second.first.fileName ) );
        
        auto t_itemSpriteSize = t_itemSprite->getContentSize();
        
        t_itemSprite->setScale( MIN( ( t_itemSize - t_itemPadding ) / t_itemSpriteSize.width, ( t_itemSize - t_itemPadding ) / t_itemSpriteSize.height ) );
        
        int t_row = (int)floor( i / t_columns );
        int t_column = i % t_columns;

        t_itemSprite->setPosition( Vec2( t_itemSize * ( t_column + 0.5f ), t_itemSize * ( t_row + 0.5f ) ) );

        addChild( t_itemSprite );
        
    }
    
    return true;
}
