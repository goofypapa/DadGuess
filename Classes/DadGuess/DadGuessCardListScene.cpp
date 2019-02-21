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
#include "DadGuessCardListScrollView.h"
#include "Common.h"
#include <map>

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
    
    std::map< std::string, std::pair< Color4F, Color4F > > t_colorList;
    std::map< std::string, std::string > t_iconList;
    
    t_colorList["animal"] = std::pair< Color4F, Color4F >( createColor4FWithStr( "9ed667" ), createColor4FWithStr( "96d15d" ) );
    t_colorList["ABC"] = std::pair< Color4F, Color4F >( createColor4FWithStr( "91eef6" ), createColor4FWithStr( "7be7f4" ) );
    t_colorList["astronomy"] = std::pair< Color4F, Color4F >( createColor4FWithStr( "a4a3e7" ), createColor4FWithStr( "9d9be4" ) );
    t_colorList["earth"] = std::pair< Color4F, Color4F >( createColor4FWithStr( "bdcdff" ), createColor4FWithStr( "b4c4f7" ) );
    t_colorList["historyChronology"] = std::pair< Color4F, Color4F >( createColor4FWithStr( "b9e4ed" ), createColor4FWithStr( "afdbe4" ) );
    t_colorList["worldHistory"] = std::pair< Color4F, Color4F >( createColor4FWithStr( "b9e4ed" ), createColor4FWithStr( "afdbe4" ) );
    
    t_iconList["animal"] = TexturePacker::DadGuessMain::caicai_xq_cover_animal;
    t_iconList["ABC"] = TexturePacker::DadGuessMain::caicai_xq_cover_abc;
    t_iconList["astronomy"] = TexturePacker::DadGuessMain::caicai_xq_cover_sky;
    t_iconList["earth"] = TexturePacker::DadGuessMain::caicai_xq_cover_earth;
    t_iconList["historyChronology"] = TexturePacker::DadGuessMain::caicai_xq_cover_chinese;
    t_iconList["worldHistory"] = TexturePacker::DadGuessMain::caicai_xq_cover_history;
    
    auto t_background = DrawNode::create();
    t_background->drawSolidRect( Vec2::ZERO , Vec2( t_visibleSizeHalf.width, t_visibleSizeHalf.height * 2.0f ), t_colorList[p_groupId].first );
    
    t_background->drawSolidRect( Vec2( t_visibleSizeHalf.width, 0.0f ) , t_visibleSizeHalf * 2.0f, t_colorList[p_groupId].second );
    t_background->setPosition( t_origin );
    
    addChild( t_background );
    
    auto t_groupLogo = Sprite::createWithSpriteFrameName( t_iconList[ p_groupId ] );
    
    auto t_groupLogoSizeHalf = t_groupLogo->getContentSize() * 0.5f;
    t_groupLogo->setScale( t_visibleSizeHalf.height * 0.6f / t_groupLogoSizeHalf.height );
    auto t_groupLogoSizeScaleHalf = t_groupLogo->getContentSize() * t_groupLogo->getScale() * 0.5f;
    auto t_groupLogoPos = Vec2( t_origin.x + t_visibleSizeHalf.width * 0.5f, t_origin.y + t_visibleSizeHalf.height * 1.05f + 15.0f );
    t_groupLogo->setPosition( t_groupLogoPos );
    addChild( t_groupLogo );
    
    if( !t_groupInfo.activation )
    {
        auto t_buyButton = DrawNode::create();
        t_buyButton->setPosition( Vec2::ZERO );
        
        auto t_buyButtonDrawBeginPos = Vec2( t_groupLogoPos.x - t_groupLogoSizeScaleHalf.width * 0.972f, t_groupLogoPos.y - t_groupLogoSizeScaleHalf.height * 0.98f );
        
        auto t_buyButtonSizeHalf = Size( t_groupLogoSizeScaleHalf.width * 0.972f, t_groupLogoSizeScaleHalf.height * 0.175f );
        
        t_buyButton->drawSolidRect( t_buyButtonDrawBeginPos, Vec2( t_groupLogoPos.x + t_buyButtonSizeHalf.width, t_buyButtonDrawBeginPos.y + t_buyButtonSizeHalf.height * 2.0f ) , Color4F( 254.0f / 255.0f, 208.0f / 255.0f, 63.0f / 255.0f, 1.0f ) );
        addChild( t_buyButton );
        
        auto t_buyLabel = MenuItemFont::create( "点击购买此套卡片", [p_groupId]( Ref * p_target ){
            
            if( p_groupId == "animal" )
            {
                goChrome( "https://h5.youzan.com/wscshop/goods/2ocwex463rym6?banner_id=f.77959558~goods~1~DwwlL4X5&reft=1550628873062&spm=f.77959558" );
            }

            if( p_groupId == "ABC" )
            {
                goChrome( "https://detail.youzan.com/show/goods?alias=2g0dmtgn565m6&banner_id=f.77959558~goods~1~8yWjakL3&reft=1550627588362&spm=f.77959558" );
            }

            if( p_groupId == "astronomy" || p_groupId == "earth" || p_groupId == "historyChronology" || p_groupId == "worldHistory" )
            {
                goChrome( "https://h5.youzan.com/wscshop/goods/3nua3ck5zx2j2?banner_id=f.77959558~goods~1~LaqUR2l3&reft=1550630144758&spm=f.77959558" );
            }
            
        } );
        t_buyLabel->setFontSizeObj( 12 );
        t_buyLabel->setPosition( t_buyButtonDrawBeginPos + t_buyButtonSizeHalf );
        t_buyLabel->setContentSize( t_buyButtonSizeHalf * 2.0f );
        
        auto t_buyLabelSizeHalf = t_buyLabel->getLabel()->getContentSize() * 0.5f;
        t_buyLabel->getLabel()->setPosition( t_buyButtonSizeHalf - t_buyLabelSizeHalf );
        
        auto t_menu = Menu::create( t_buyLabel, NULL);
        t_menu->setPosition( Vec2::ZERO );
        addChild( t_menu );
   
    
        // auto t_unlockPromptTitle = Label::createWithSystemFont( "# 扫卡解锁 #", "", 12 );
        // auto t_unlockPromptTitlePos = Vec2( t_groupLogoPos.x, t_groupLogoPos.y - t_groupLogoSizeScaleHalf.height - t_unlockPromptTitle->getContentSize().height * 0.5f - 2.0f );
        // t_unlockPromptTitle->setPosition( t_unlockPromptTitlePos );
        // addChild( t_unlockPromptTitle );
        
        // auto t_unlockPrompt = Label::createWithSystemFont( "用此套卡片里的任意一张卡片在小鼓上方扫过即可激活该套所有卡片" , "", 10 );
        // t_unlockPrompt->setDimensions( t_visibleSizeHalf.width * 0.8f , 30.0f );
        // t_unlockPrompt->setPosition( Vec2( t_groupLogoPos.x, t_unlockPromptTitlePos.y - 24.0f ) );
        // t_unlockPrompt->setAlignment( TextHAlignment::CENTER );
        // addChild( t_unlockPrompt );
    }
    
    auto t_contentPromptTitle = Label::createWithSystemFont( "内容简介", "", 12 );
    auto t_contentPromptTitleSizeHalf = t_contentPromptTitle->getContentSize() * 0.5f;
    auto t_contentPromptTitlePos = Vec2( t_origin.x + t_visibleSizeHalf.width * 0.5f, t_groupLogoPos.y - t_groupLogoSizeHalf.height * t_groupLogo->getScale() - t_contentPromptTitleSizeHalf.height - 5.0f );
    t_contentPromptTitle->setPosition( t_contentPromptTitlePos );
    
    addChild( t_contentPromptTitle );
    
    auto t_contentPrompt = Label::createWithSystemFont( t_groupInfo.synopsis, "", 10 );
    auto t_contentPromptSize = Size( t_visibleSizeHalf.width * 0.8f , 50.0f );
    auto t_contentPromptPos = Vec2( t_contentPromptTitlePos.x, t_contentPromptTitlePos.y - t_contentPromptTitleSizeHalf.height - 30.0f  );
    t_contentPrompt->setDimensions( t_contentPromptSize.width , t_contentPromptSize.height );
    t_contentPrompt->setPosition( t_contentPromptPos );
    addChild( t_contentPrompt );
    
    auto t_cardPreviewTitle = Label::createWithSystemFont( "卡片预览", "", 12 );
    auto t_cardPreviewTitleSizeHalf = t_cardPreviewTitle->getContentSize() * 0.5f;
    auto t_cardPreviewTitlePos = Vec2( t_origin.x + t_visibleSizeHalf.width * 1.5f, t_origin.y + t_visibleSizeHalf.height * 2.0f - t_cardPreviewTitleSizeHalf.height - 20.0f );
    t_cardPreviewTitle->setPosition( t_cardPreviewTitlePos );
    
    addChild( t_cardPreviewTitle );
    
    auto t_cardPreviewTitleLine = DrawNode::create();
    t_cardPreviewTitleLine->drawLine( Vec2( t_origin.x + t_visibleSizeHalf.width * 1.05f, t_cardPreviewTitlePos.y ) , Vec2( t_origin.x + t_cardPreviewTitlePos.x - t_visibleSizeHalf.width * 0.03f - t_cardPreviewTitleSizeHalf.width , t_cardPreviewTitlePos.y ), Color4F( 1.0f, 1.0f, 1.0f, 1.0f ) );
    
    t_cardPreviewTitleLine->drawLine(  Vec2( t_origin.x + t_cardPreviewTitlePos.x + t_visibleSizeHalf.width * 0.03f + t_cardPreviewTitleSizeHalf.width , t_cardPreviewTitlePos.y ), Vec2( t_origin.x + t_visibleSizeHalf.width * 2.0f - t_visibleSizeHalf.width * 0.05f, t_cardPreviewTitlePos.y ), Color4F( 1.0f, 1.0f, 1.0f, 1.0f ) );
    
    addChild( t_cardPreviewTitleLine );
    
    
    auto t_cardListPadding = t_visibleSizeHalf.width * 0.05f;
//    auto t_cardListArea = DrawNode::create();
//    t_cardListArea->drawRect( Vec2( t_origin.x + t_visibleSizeHalf.width + t_cardListPadding, t_origin.y + t_cardListPadding ) , Vec2( t_origin.x + t_visibleSizeHalf.width * 2.0f - t_cardListPadding, t_cardPreviewTitlePos.y - t_cardPreviewTitleSizeHalf.height - 3.0f ), Color4F( 1.0f, 0.0f, 0.0f, 1.0f ) );
//
//    addChild( t_cardListArea );
    
    auto t_cardListScrollView = DadGuessCardListScrollView::createWithSize( Size( t_visibleSizeHalf.width - t_cardListPadding * 2.0f, t_cardPreviewTitlePos.y - t_cardPreviewTitleSizeHalf.height - 5.0f - t_origin.y - t_cardListPadding ), p_groupId );
    
    t_cardListScrollView->setPosition( Vec2( t_origin.x + t_visibleSizeHalf.width + t_cardListPadding, t_origin.y + t_cardListPadding ) );
    
    addChild( t_cardListScrollView );
    
    auto t_btnBack = Button::create( TexturePacker::DadGuessMain::caicai_back, TexturePacker::DadGuessMain::caicai_back, "", Button::TextureResType::PLIST );
    t_btnBack->setScale( adaptation() );
    auto t_btnBackSizeHalf = t_btnBack->getContentSize() * t_btnBack->getScale() * 0.5f;
    t_btnBack->setPosition( Vec2( t_origin.x + t_btnBackSizeHalf.width + 15.0f, t_origin.y + t_visibleSizeHalf.height * 2.0f - t_btnBackSizeHalf.height - 15.0f ) );
    addChild( t_btnBack );
    
    touchAnswer( t_btnBack , [t_cardListScrollView]( Ref * p_target ){
        t_cardListScrollView->cancelDownloadImage();
        Director::getInstance()->replaceScene( DadGuessMainScene::create() );
    }, adaptation() * 1.2f, adaptation() );
    
    return true;
}
