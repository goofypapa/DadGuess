//
//  DadGuessCardListScrollView.cpp
//  DadGuess-mobile
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

std::list< Http * > DadGuessCardListScrollView::sm_invalidDownloadList;
std::list< Http * > DadGuessCardListScrollView::sm_downloadingList;

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
        
        
        Sprite * t_itemSprite = nullptr;
        if( t_itemTexture )
        {
            t_itemSprite = Sprite::createWithTexture( t_itemTexture );
        }else
        {
            auto t_fileInfo = DataTableFile::instance().findBySourceUrl( t_cardList[i].first.coverFileUrl );
            
            if( !t_fileInfo.fileId.empty() )
            {
                t_itemTexture = Director::getInstance()->getTextureCache()->getTextureForKey( t_fileInfo.fileName );
                if( !t_itemTexture )
                {
                    t_itemTexture = Director::getInstance()->getTextureCache()->addImage( t_fileInfo.fileName );
                }
                t_itemSprite = Sprite::createWithTexture( t_itemTexture );
            }else
            {
                const char * t_key = "DadGuess/LoadCard.png";
                t_itemTexture = Director::getInstance()->getTextureCache()->getTextureForKey( t_key );
                if( !t_itemTexture )
                {
                    t_itemTexture = Director::getInstance()->getTextureCache()->addImage( "DadGuess/LoadCard.png" );
                }
                t_itemSprite = Sprite::createWithTexture( t_itemTexture );
                
                m_downloadPool.push( std::pair< std::string, std::function< void( DataFileInfo ) > >( t_cardList[i].first.coverFileUrl, [t_itemSprite]( DataFileInfo p_file ){
                    
                        auto director = Director::getInstance();
                        Scheduler *sched = director->getScheduler();
                        sched->performFunctionInCocosThread( [=](){
                            t_itemSprite->setTexture( p_file.fileName );
                        } );
                } ) );
            }
        }

        auto t_itemSpriteSize = t_itemSprite->getContentSize();
        
        t_itemSprite->setScale( MIN( ( t_itemSize - t_itemPadding ) / t_itemSpriteSize.width, ( t_itemSize - t_itemPadding ) / t_itemSpriteSize.height ) );
        
        int t_row = (int)floor( i / sm_columns );
        int t_column = i % sm_columns;

        t_itemSprite->setPosition( Vec2( t_itemSize * ( t_column + 0.5f ), t_innerContainerHeight - t_itemSize * ( t_row + 0.5f ) ) );

        addChild( t_itemSprite );
        
    }
    
    loadImage();
    
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

void DadGuessCardListScrollView::loadImage( void )
{
    if( m_downloadPool.size() )
    {
        auto t_download = m_downloadPool.front();
        
        sm_downloadingList.push_back( Http::DownloadFile( t_download.first, "", [this, t_download]( Http * p_http, DataFileInfo p_file ){
            
            auto t_findInvalidDownloadList = std::find( sm_invalidDownloadList.begin(), sm_invalidDownloadList.end(), p_http );
            
            if( t_findInvalidDownloadList != sm_invalidDownloadList.end() )
            {
                sm_invalidDownloadList.erase( t_findInvalidDownloadList );
                return;
            }
            
            auto t_findDownloadListIt = std::find( sm_downloadingList.begin(), sm_downloadingList.end(), p_http );
            if( t_findDownloadListIt != sm_downloadingList.end() )
            {
                sm_downloadingList.erase( t_findDownloadListIt );
            }
            
            t_download.second( p_file );
            
            m_downloadPool.pop();
            
            loadImage();
        }, [this, t_download]( Http * p_http, DataFileInfo p_file ){
            
            auto t_findDownloadListIt = std::find( sm_downloadingList.begin(), sm_downloadingList.end(), p_http );
            if( t_findDownloadListIt != sm_downloadingList.end() )
            {
                sm_downloadingList.erase( t_findDownloadListIt );
            }
            
            m_downloadPool.pop();
            loadImage();
        }) );
    }
}

DadGuessCardListScrollView::~DadGuessCardListScrollView()
{
    if( sm_downloadingList.size() )
    {
        for( auto item : sm_downloadingList )
        {
            sm_invalidDownloadList.push_back( item );
        }
    }
    
    sm_downloadingList.clear();
}
