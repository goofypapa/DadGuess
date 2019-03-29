//
//  DadGuessCardListScene.cpp
//  DadGuess-mobile
//
//  Created by 吴思 on 2018/11/21.
//

#include "DadGuessCardListScene.h"
#include "DadGuessMainScene.h"
#include "DadGuessMain.hpp"
#include "DadGuessUpdateScene.h"
#include "Common.h"
#include <map>
#include "DadGuessCardListScrollView.h"

#include "DataTableCardBatch.h"
#include "DataTableFile.h"
#include "DataTableCard.h"

#include "ui/CocosGUI.h"


USING_NS_CC;
using namespace cocos2d::ui;


DadGuessCardListScene * DadGuessCardListScene::createScene( const std::string & p_groupId )
{
    auto t_result = create();
    
    if( !t_result->initWithGroupId( p_groupId ) )
    {
        t_result = nullptr;
    }
    
    return t_result;
}

bool DadGuessCardListScene::init( void )
{
    if( !BaseScene::init() )
    {
        return false;
    }
    
    return true;
}

bool DadGuessCardListScene::initWithGroupId( const std::string & p_groupId )
{
    auto t_visibleSizeHalf = Director::getInstance()->getVisibleSize() * 0.5f;
    Vec2 t_origin = Director::getInstance()->getVisibleOrigin();
    
    auto t_groupInfo = DataTableCardBatch::instance().find( p_groupId );
    
    std::map< std::string, std::vector< std::string > > t_dataInfo;

    t_dataInfo["animal"] = std::vector< std::string >( { "9ed667", "动物百科", TexturePacker::DadGuessMain::guess_card_list_bg_animal, TexturePacker::DadGuessMain::guess_card_group_icon_animal } );
    t_dataInfo["ABC"] = std::vector< std::string >( { "74b4f4", "ABC", TexturePacker::DadGuessMain::guess_card_list_bg_ABC, TexturePacker::DadGuessMain::guess_card_group_icon_ABC } );
    t_dataInfo["astronomy"] = std::vector< std::string >( { "8789ed", "天文", TexturePacker::DadGuessMain::guess_card_list_bg_astronomy, TexturePacker::DadGuessMain::guess_card_group_icon_astronomy } );
    t_dataInfo["earth"] = std::vector< std::string >( { "75cff2", "地球", TexturePacker::DadGuessMain::guess_card_list_bg_earth, TexturePacker::DadGuessMain::guess_card_group_icon_earth} );
    t_dataInfo["historyChronology"] = std::vector< std::string >( { "f4ad79", "中国历史", TexturePacker::DadGuessMain::guess_card_list_bg_historyChronology, TexturePacker::DadGuessMain::guess_card_group_icon_historyChronology } );
    t_dataInfo["worldHistory"] = std::vector< std::string >( { "f5c570", "世界历史", TexturePacker::DadGuessMain::guess_card_list_bg_worldHistory, TexturePacker::DadGuessMain::guess_card_group_icon_worldHistory } );
    
    auto t_background = DrawNode::create();
    t_background->drawSolidRect( Vec2::ZERO , Vec2( t_visibleSizeHalf.width * 2.0f, t_visibleSizeHalf.height * 2.0f ), createColor4FWithStr( t_dataInfo[p_groupId][0] ) );
    t_background->setPosition( t_origin );

    addChild( t_background );

    auto t_btnBack = Button::create( TexturePacker::DadGuessMain::guess_card_list_back, TexturePacker::DadGuessMain::guess_card_list_back, "", Button::TextureResType::PLIST );
    t_btnBack->setScale( adaptation() );
    auto t_btnBackSizeHalf = t_btnBack->getContentSize() * t_btnBack->getScale() * 0.5f;
    t_btnBack->setPosition( Vec2( t_origin.x + t_btnBackSizeHalf.width + 15.0f, t_origin.y + t_visibleSizeHalf.height * 2.0f - t_btnBackSizeHalf.height - 15.0f ) );
    addChild( t_btnBack );

    auto t_cardGroupIcon = Button::create( t_dataInfo[p_groupId][3], t_dataInfo[p_groupId][3], "", Button::TextureResType::PLIST );
    t_cardGroupIcon->setScale( adaptation() );
    auto t_cardGroupIconSizeHalf = t_cardGroupIcon->getContentSize() * t_cardGroupIcon->getScale() * 0.5f;
    t_cardGroupIcon->setPosition( Vec2( t_origin.x + t_visibleSizeHalf.width * 2.0f - t_cardGroupIconSizeHalf.width - 15.0f, t_origin.y + t_visibleSizeHalf.height * 2.0f - t_cardGroupIconSizeHalf.height - 15.0f ) );

    if( !t_groupInfo.activation )
    {
        auto t_buyButton = Button::create( TexturePacker::DadGuessMain::buyButton, TexturePacker::DadGuessMain::buyButton, "", Button::TextureResType::PLIST );
        t_buyButton->setScale( adaptation() );
        auto t_buyButtonSizeHalf = t_buyButton->getContentSize() * t_buyButton->getScale() * 0.5f;
        t_buyButton->setPosition( Vec2( t_origin.x + t_visibleSizeHalf.width * 2.0f - t_buyButtonSizeHalf.width - 25.0f, t_origin.y + t_visibleSizeHalf.height * 2.0f - t_cardGroupIconSizeHalf.height - 15.0f ) );
        
        addChild( t_buyButton );

        auto t_groupId = p_groupId;
        touchAnswer( t_buyButton, [t_groupId]( Ref * p_target ){

            if( t_groupId == "animal" )
            {
                goChrome( "https://h5.youzan.com/wscshop/goods/2ocwex463rym6?banner_id=f.77959558~goods~1~DwwlL4X5&reft=1550628873062&spm=f.77959558" );
            }

            if( t_groupId == "ABC" )
            {
                goChrome( "https://detail.youzan.com/show/goods?alias=2g0dmtgn565m6&banner_id=f.77959558~goods~1~8yWjakL3&reft=1550627588362&spm=f.77959558" );
            }

            if( t_groupId == "astronomy" || t_groupId == "earth" || t_groupId == "historyChronology" || t_groupId == "worldHistory" )
            {
                goChrome( "https://h5.youzan.com/wscshop/goods/3nua3ck5zx2j2?banner_id=f.77959558~goods~1~LaqUR2l3&reft=1550630144758&spm=f.77959558" );
            }
        }, adaptation() * 1.1f, adaptation() );
    }

    addChild( t_cardGroupIcon );


    auto t_cardGroupNameLabel = Label::createWithSystemFont( t_dataInfo[p_groupId][1], "", 14.0f );
    auto t_cardGroupNameLabelSizeHalf = t_cardGroupNameLabel->getContentSize() * 0.5f;
    float t_cardGroupNameLabelPosY = t_origin.y + t_visibleSizeHalf.height * 2.0f - t_cardGroupNameLabelSizeHalf.height - 30.0f;
    t_cardGroupNameLabel->setPosition( Vec2( t_origin.x + t_visibleSizeHalf.width, t_cardGroupNameLabelPosY ) );
    addChild( t_cardGroupNameLabel );

    auto t_drawLine = DrawNode::create();

    float t_leftLineStartPosX = t_origin.x + t_visibleSizeHalf.width - t_cardGroupNameLabelSizeHalf.width * 2.0f;
    float t_rightLineStartPos = t_origin.x + t_visibleSizeHalf.width + t_cardGroupNameLabelSizeHalf.width * 2.0f;
    t_drawLine->drawLine( Vec2( t_leftLineStartPosX - 50.0f, t_cardGroupNameLabelPosY ), Vec2( t_leftLineStartPosX, t_cardGroupNameLabelPosY ), createColor4FWithStr("FFFFFF") );
    t_drawLine->drawLine( Vec2( t_rightLineStartPos, t_cardGroupNameLabelPosY ), Vec2( t_rightLineStartPos + 50.0f, t_cardGroupNameLabelPosY ), createColor4FWithStr("FFFFFF") );
    addChild( t_drawLine );

    auto t_cardGroupSynopsis = Label::createWithSystemFont( t_groupInfo.synopsis, "", 10.0f );
    auto t_cardGroupSynopsisSize = Size( t_visibleSizeHalf.width * 1.6f , 25.0f );
    auto t_cardGroupSynopsisPos = Vec2( t_origin.x + t_visibleSizeHalf.width, t_origin.y + t_visibleSizeHalf.height * 2.0f - t_cardGroupNameLabelSizeHalf.height * 2.0f - t_cardGroupSynopsisSize.height - 20.0f );

    t_cardGroupSynopsis->setDimensions( t_cardGroupSynopsisSize.width , t_cardGroupSynopsisSize.height );
    t_cardGroupSynopsis->setPosition( t_cardGroupSynopsisPos );

    t_cardGroupSynopsis->setHorizontalAlignment(TextHAlignment::CENTER );

    addChild( t_cardGroupSynopsis );


    auto t_cardListBg = Scale9Sprite::createWithSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( t_dataInfo[p_groupId][2] ) );
    
    auto t_boxSize = Size( t_visibleSizeHalf.width * 1.8f, ( t_visibleSizeHalf.height - t_cardGroupNameLabelSizeHalf.height ) * 2.0f - 70.0f );
    
    auto t_cardListBgSize = Size( t_boxSize.width, t_boxSize.height - 10.0f );
    t_cardListBg->setContentSize( t_cardListBgSize );
    auto t_cardListBgPos = Vec2( t_origin.x + t_visibleSizeHalf.width, t_origin.y + t_boxSize.height * 0.5f + 10.0f );
    t_cardListBg->setPosition( t_cardListBgPos );
    addChild( t_cardListBg );

    // auto t_testArea = DrawNode::create();
    // t_testArea->drawSolidRect( Vec2( 15.0f, 15.0f ), Vec2( t_cardListBgSize.width - 15.0f, t_cardListBgSize.height - 15.0f ), createColor4FWithStr( "FFFFFF", 0.4f ) );
    // t_testArea->drawSolidRect( Vec2( 3.0f, 3.0f ), Vec2( t_cardListBgSize.width - 3.0f, t_cardListBgSize.height - 3.0f ), createColor4FWithStr( "ABCDEF", 0.6f ) );
    // t_cardListBg->addChild( t_testArea );


    auto t_cardListScrollView = DadGuessCardListScrollView::createWithSize( Size(  t_cardListBgSize.width + 20.0f, t_cardListBgSize.height - 6.0f ), p_groupId );
    
    t_cardListScrollView->setPosition( Vec2( -10.0f, 3.0f ) );
    
    t_cardListBg->addChild( t_cardListScrollView );


    touchAnswer( t_btnBack , [t_cardListScrollView]( Ref * p_target ){
        t_cardListScrollView->cancelDownloadImage();
        Director::getInstance()->replaceScene( DadGuessMainScene::create() );
    }, adaptation() * 1.2f, adaptation() );

    return true;
}