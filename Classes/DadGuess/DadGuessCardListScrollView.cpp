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

int DadGuessCardListScrollView::sm_columns = 6;

std::list< Http * > DadGuessCardListScrollView::sm_invalidDownloadList;
std::list< Http * > DadGuessCardListScrollView::sm_downloadingList;
float DadGuessCardListScrollView::s_percentVertical = 0.0f;

std::mutex DadGuessCardListScrollView::sm_downloadMutex;

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

    m_cancelDownload = false;
    
    return true;
}

bool DadGuessCardListScrollView::initWithSize( const cocos2d::Size & p_contentSize, const std::string p_groupId )
{
    setContentSize( p_contentSize );
    
    m_groupId = p_groupId;
    auto & t_cardList = DadGuessUpdateScene::s_cardList[m_groupId];
    

    m_paddingTop = 11.0f;
    m_paddingH = 15.0f;

    int t_rowCount = (int)ceil(t_cardList.size() / (float)sm_columns);

    m_groudPadding = 10.0f;
    m_groudWidth = ( p_contentSize.width - m_paddingH * 2.0f - m_groudPadding ) / sm_columns - m_groudPadding;
    m_groudHeight = m_groudWidth * 1.36f;

    m_innerContainerSize = Size( p_contentSize.width, m_paddingTop + t_rowCount * m_groudHeight + m_groudPadding * t_rowCount ); 
    
    setInnerContainerSize( m_innerContainerSize );
    
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
        t_itemSprite->setScale( m_groudHeight / t_itemSpriteSize.height );
        auto t_itemSpriteRealSizeHalf = Size( t_itemSpriteSize.width * t_itemSprite->getScale(), t_itemSpriteSize.height * t_itemSprite->getScale() ) * 0.5f;
        int t_row = (int)floor( i / sm_columns );
        int t_column = i % sm_columns;

        t_itemSprite->setPosition( Vec2( m_paddingH + m_groudWidth * ( t_column + 0.5f ) + m_groudPadding * ( t_column + 1 ), m_innerContainerSize.height - m_groudHeight * ( t_row + 0.5f ) - ( m_groudPadding * t_row + 1 ) + ( p_contentSize.height > m_innerContainerSize.height ? p_contentSize.height - m_innerContainerSize.height : 0.0f ) - m_paddingTop ) );

        addChild( t_itemSprite );
        
    }
    
    loadImage();
    
    return true;
}

void DadGuessCardListScrollView::onEnter()
{
    ui::ScrollView::onEnter();
    jumpToPercentVertical( isnormal( s_percentVertical ) ? s_percentVertical : 0.0f );
}

void DadGuessCardListScrollView::onTouched( const int p_index )
{
    auto & t_cardList = DadGuessUpdateScene::s_cardList[m_groupId];
    
    if( p_index >= t_cardList.size() )
    {
        return;
    }

    if( m_groupId.compare( DataCardBatchInfo::s_batchIdList[0] ) == 0 )
    {
        setAppOrientation( true );
    }

    cancelDownloadImage();
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
    s_percentVertical = getScrolledPercentVertical();
    s_percentVertical = isnormal( s_percentVertical ) ? s_percentVertical : 0.0f;
    auto t_beginPos = m_touchBeginLocationList[p_touch->getID()];
    m_touchBeginLocationList.erase( p_touch->getID() );

    if( t_beginPos.distance( p_touch->getLocation() ) < 2.0f )
    {
        auto t_coutentHieght = getContentSize().height;
        auto t_postion = convertToNodeSpace( t_beginPos );
        auto t_offsetV = ( s_percentVertical / 100.0f ) * ( getInnerContainerSize().height - t_coutentHieght ) ;
        
        auto t_realPos = Vec2( t_postion.x - m_paddingH , getContentSize().height - t_postion.y + t_offsetV - m_paddingTop );


        auto t_itemWidth = m_groudWidth + m_groudPadding;
        auto t_itemHeight = m_groudHeight + m_groudPadding;

        if( t_realPos.x < 0 || t_realPos.x >= t_itemWidth * sm_columns || t_realPos.y < 0 )
        {
            return;
        }

        if( fmod( t_realPos.x, t_itemWidth ) < m_groudPadding || fmod( t_realPos.y, t_itemHeight ) > m_groudHeight )
        {
            return;
        }

        auto t_contentSize = getContentSize();
        
        int t_row = (int)floor( t_realPos.y / t_itemHeight );
        int t_column = (int)floor( t_realPos.x / t_itemWidth );
        
        auto t_selectIndex = t_row * sm_columns + t_column;
        
        onTouched( t_selectIndex );
    }
}

void DadGuessCardListScrollView::loadImage( void )
{

    sm_downloadMutex.lock();
    if( m_cancelDownload )
    {
        sm_downloadMutex.unlock();
        return;
    }

    if( m_downloadPool.size() )
    {
        auto t_download = m_downloadPool.front();
        
        sm_downloadingList.push_back( Http::DownloadFile( t_download.first, "", [this, t_download]( Http * p_http, DataFileInfo p_file ){
                        
            sm_downloadMutex.lock();
            auto t_findInvalidDownloadList = std::find( sm_invalidDownloadList.begin(), sm_invalidDownloadList.end(), p_http );
            
            if( t_findInvalidDownloadList != sm_invalidDownloadList.end() )
            {
                sm_invalidDownloadList.erase( t_findInvalidDownloadList );
                sm_downloadMutex.unlock();
                return;
            }
            
            auto t_findDownloadListIt = std::find( sm_downloadingList.begin(), sm_downloadingList.end(), p_http );
            if( t_findDownloadListIt != sm_downloadingList.end() )
            {
                sm_downloadingList.erase( t_findDownloadListIt );
            }
            
            t_download.second( p_file );

            if( m_cancelDownload )
            {
                sm_downloadMutex.unlock();
                return;
            }

            sm_downloadMutex.unlock();

            m_downloadPool.pop();
            
            loadImage();
        }, [this, t_download]( Http * p_http, DataFileInfo p_file ){
            sm_downloadMutex.lock();
            auto t_findDownloadListIt = std::find( sm_downloadingList.begin(), sm_downloadingList.end(), p_http );
            if( t_findDownloadListIt != sm_downloadingList.end() )
            {
                sm_downloadingList.erase( t_findDownloadListIt );
            }
            sm_downloadMutex.unlock();

            if( m_cancelDownload )
            {
                return;
            }

            m_downloadPool.pop();
            loadImage();
        }) );
    }

    sm_downloadMutex.unlock();
}

bool DadGuessCardListScrollView::cancelDownloadImage( void )
{
    sm_downloadMutex.lock();
    m_cancelDownload = true;
    if( sm_downloadingList.size() )
    {
        for( auto item : sm_downloadingList )
        {
            sm_invalidDownloadList.push_back( item );
        }
    }

    sm_downloadMutex.unlock();
    
    return true;
}
