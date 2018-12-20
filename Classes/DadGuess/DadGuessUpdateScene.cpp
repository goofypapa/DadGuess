//
//  DadGuessUpdateScene.cpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/10/29.
//

#include "DadGuessUpdateScene.h"
#include "DadGuessUpdate.hpp"
#include "Common.h"

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

#include "Config.h"
#include "DadGuessMainScene.h"
#include <unistd.h>
#include <thread>
#include "DataTableKeyValue.h"

USING_NS_CC;
using namespace rapidjson;

const char * DadGuessUpdateScene::sm_batchListApi = DOMAIN_NAME "resource/batch/list/summary.do";
const char * DadGuessUpdateScene::sm_cardListApi = DOMAIN_NAME "api/card/res/list.do";
const char * DadGuessUpdateScene::sm_cardListApi2 = DOMAIN_NAME "api/card/app/list.do";
const char * DadGuessUpdateScene::sm_cardTypeListApi = DOMAIN_NAME "api/card/app/type/list.do";

const char * DadGuessUpdateScene::s_checkUpdateKey = "DadGuessCardCheckUpdate";
const long DadGuessUpdateScene::s_updateOverTime = 60 * 60 * 24 * 2;

bool DadGuessUpdateScene::s_updateed = false;

std::vector< std::string > DadGuessUpdateScene::sm_loadImageList;
std::vector< DataCardBatchInfo > DadGuessUpdateScene::sm_cardBatchList;

std::map< std::string, std::vector< std::pair< DataCardInfo, DataFileInfo > > > DadGuessUpdateScene::s_cardList;

bool DadGuessUpdateScene::init( void )
{
    if( !BaseScene::init() )
    {
        return false;
    }

    auto t_loginUser = DataTableUser::instance().getActivation();
    Http::token = t_loginUser.token;

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile( "DadGuess/DadGuessUpdate.plist", Director::getInstance()->getTextureCache()->addImage( "DadGuess/DadGuessUpdate.png" ) );
    
    auto t_visibleSizeHalf = Director::getInstance()->getVisibleSize() * 0.5f;
    Vec2 t_origin = Director::getInstance()->getVisibleOrigin();
    
    auto t_centerPos = Vec2( t_origin.x + t_visibleSizeHalf.width, t_origin.y + t_visibleSizeHalf.height );
    
    DrawNode * t_backgroundColor = DrawNode::create();
    
    t_backgroundColor->setPosition( Vec2( t_origin.x + t_visibleSizeHalf.width, t_origin.y + t_visibleSizeHalf.height ) );
    
    t_backgroundColor->drawSolidRect( Vec2( -t_visibleSizeHalf.width, -t_visibleSizeHalf.height ), Vec2( t_visibleSizeHalf.width, t_visibleSizeHalf.height ), Color4F( 122.0f / 255.0f, 212.0f / 255.0f, 249.0f / 255.0f, 1.0f ) );
    
    addChild( t_backgroundColor );
    
    m_hand = TexturePacker::DadGuessUpdate::createHeadSprite();
    m_hand->setScale( adaptation() );
    auto t_headSizeHalf = m_hand->getContentSize() * 0.5f;
    auto t_headPos = t_centerPos + Vec2( 0.0f, t_visibleSizeHalf.height * 0.2f );
    m_hand->setPosition( t_headPos );
    addChild( m_hand );
    
    auto t_dot = TexturePacker::DadGuessUpdate::createDotSprite();
    float t_dotDistance = t_headSizeHalf.width * 1.5f;
    t_dot->setPosition( Vec2( t_headSizeHalf.width, t_headSizeHalf.height ) );
    m_hand->addChild( t_dot );
    
    auto  t_eyeDistance = t_headSizeHalf.width * 0.07f;
    auto t_eyeLeft = TexturePacker::DadGuessUpdate::createEyeSprite();
    auto t_eyeLeftPos = Vec2( t_headSizeHalf.width * 0.73f, t_headSizeHalf.height * 0.885f );
    t_eyeLeft->setPosition( t_eyeLeftPos );
    m_hand->addChild( t_eyeLeft );
    
    auto t_eyeRight = TexturePacker::DadGuessUpdate::createEyeSprite();
    auto t_eyeRightPos = Vec2( t_headSizeHalf.width * 1.27f, t_headSizeHalf.height * 0.885f );
    t_eyeRight->setPosition( t_eyeRightPos );
    m_hand->addChild( t_eyeRight );
    
    
    m_hand->runAction( RepeatForever::create( ActionFloat::create( 3.0f , 0.0f, 1.0f, [=]( const float t_dt ){
        float t_seed = t_dt * PI * -2.0f;
        t_dot->setPosition( Vec2( t_headSizeHalf.width + cos( t_seed ) * t_dotDistance, t_headSizeHalf.height + sin( t_seed ) * t_dotDistance ) );
        t_dot->setRotation( t_dt * 360.0f );
        
        auto t_eyeOffset = Vec2( cos( t_seed ) * t_eyeDistance, sin( t_seed ) * t_eyeDistance );
        t_eyeLeft->setPosition( t_eyeLeftPos + t_eyeOffset );
        t_eyeRight->setPosition( t_eyeRightPos + t_eyeOffset );
        
    }) ) );
    
    
    //更新内容提示
    m_messageLabel = Label::createWithSystemFont( "正在检查更新..." , "", 12 );
    m_messageLabel->setPosition( t_headPos - Vec2( 0.0f, t_dotDistance * adaptation() * 2.0f ) );
    addChild( m_messageLabel );
    
    
    //检查更新
    
    //更新批次
    auto t_checkCardBatchUpdate = [this]( void ){
        
        m_messageLabel->setString( "正在检查卡片批次更新..." );

        Http::HttpParameter t_parameter;
        
        m_checkUpdateHandlerList.push_back( Http::Post( sm_cardTypeListApi , &t_parameter, [this]( Http * p_http, std::string p_res ){
            Document t_readdoc;
            
            t_readdoc.Parse<0>( p_res.c_str() );
            
            if( t_readdoc.HasParseError() )
            {
                printf( "GetParseError %d \n", t_readdoc.GetParseError() );
            }
            
            auto t_success = t_readdoc["success"].GetBool();
            if( !t_success )
            {
                printf( "------------> 检查更新失败" );
                return;
            }
            
            auto & t_data = t_readdoc["data"];
            
            //更新批次
            for( int i = 0; i < t_data.Capacity(); ++i )
            {
                auto & t_item = t_data[i];
                
                DataCardBatchInfo t_serviceCardBatchInfo( t_item["typeName"].GetString(),
                                                         t_item["typeDesc"].IsNull() ? "" : t_item["typeDesc"].GetString()
                                                         );
                auto t_batchInfo = DataTableCardBatch::instance().find( t_serviceCardBatchInfo.id );
                if( t_batchInfo.id.empty() )
                {
                    DataTableCardBatch::instance().insert( t_serviceCardBatchInfo );
                }else if( t_serviceCardBatchInfo.synopsis.compare( t_batchInfo.synopsis ) != 0 ){
                    DataTableCardBatch::instance().update( t_serviceCardBatchInfo );
                }
            }
            
            {
                m_mutex.lock();
                checkUpdateResponse( p_http );
                m_mutex.unlock();
            }
            
        }, [this]( Http * p_http, std::string p_res ){
            m_messageLabel->setString( "检查卡片批次更新失败 请检查网络!" );
            
            auto t_it = std::find( m_checkUpdateHandlerList.begin(), m_checkUpdateHandlerList.end(), p_http );
            
            if( t_it != m_checkUpdateHandlerList.end() )
            {
                m_checkUpdateHandlerList.erase( t_it );
            }
        } ) );
        
        
        m_checkUpdateHandlerList.push_back( Http::Post( sm_batchListApi , &t_parameter, [this]( Http * p_http, std::string p_res ){
            Document t_readdoc;
            
            m_mutex.lock();
            t_readdoc.Parse<0>( p_res.c_str() );
            m_mutex.unlock();
            
            if( t_readdoc.HasParseError() )
            {
                printf( "GetParseError %d \n", t_readdoc.GetParseError() );
            }
            
            auto t_success = t_readdoc["success"].GetBool();
            if( !t_success )
            {
                printf( "------------> 检查更新失败" );
                return;
            }
            
            auto & t_data = t_readdoc["data"];
            
            //更新批次
            for( int i = 0; i < t_data.Capacity(); ++i )
            {
                auto & t_item = t_data[i];
                bool t_isUsable = t_item["isUsable"].GetBool();
                if( !t_isUsable )
                {
                    continue;
                }

                DataCardBatchInfo t_serviceCardBatchInfo( t_item["batchId"].GetString(),
                                                         t_item["batchDesc"].IsNull() ? "" : t_item["batchDesc"].GetString()
                                                         );
                
                sm_cardBatchList.push_back( t_serviceCardBatchInfo );
                
            }
            
            {
                m_mutex.lock();
                checkUpdateResponse( p_http );
                m_mutex.unlock();
            }
            
        }, [this]( Http * p_http, std::string p_res ){
            m_messageLabel->setString( "检查卡片批次更新失败 请检查网络!" );
            
            auto t_it = std::find( m_checkUpdateHandlerList.begin(), m_checkUpdateHandlerList.end(), p_http );
            
            if( t_it != m_checkUpdateHandlerList.end() )
            {
                m_checkUpdateHandlerList.erase( t_it );
            }
        } ) );
    };
    
    
    //更新卡片
    auto t_checkCardUpdate = [this](){

        //更新动物卡片
        m_messageLabel->setString( "正在检查卡片更新..." );
        
        Http::HttpParameter t_parameter;

        auto t_http = Http::Post( sm_cardListApi, &t_parameter, [this]( Http * p_http, std::string p_res ){

            auto t_batchId = m_checkCardUpdateBatchIdList[p_http];

            auto t_cardBatchInfo = DataTableCardBatch::instance().find( t_batchId );

            auto t_localCardList = DataTableCard::instance().list( t_batchId );
            m_checkCardUpdateBatchIdList.erase( p_http );

            Document t_readdoc;

            m_mutex.lock();
            t_readdoc.Parse<0>( p_res.c_str() );
            m_mutex.unlock();

            if( t_readdoc.HasParseError() )
            {
                printf( "GetParseError %d \n", t_readdoc.GetParseError() );
            }

            auto t_success = t_readdoc["success"].GetBool();
            if( !t_success )
            {
                printf( "------------> 检查更新失败" );
                return;
            }

            auto & t_data = t_readdoc["data"];

            //更新动物卡片
            for( int i = 0; i < t_data.Capacity(); ++i )
            {
                auto & t_item = t_data[i];

                std::string t_coverImageUrl = "";
                std::string t_coverImageMd5 = "";

                if( t_item["coverImage"].IsObject() )
                {
                    std::stringstream t_sstr;
                    t_coverImageUrl = std::string( DOMAIN_NAME ) + t_item["coverImage"]["attUrl"].GetString();
                    t_coverImageMd5 = t_item["coverImage"]["md5"].GetString();
                }

                auto t_cardInfo = DataCardInfo( t_item["resourceId"].GetString(),
                                               t_item["ownerId"].GetString(),
                                               t_item["rfId"].GetInt(),
                                               t_coverImageUrl,
                                               t_coverImageMd5
                                               );

                bool t_batchIsExist = false;
                for( auto t_batchInfo : sm_cardBatchList )
                {
                    if( t_batchInfo.id.compare( t_cardInfo.batchId ) == 0 )
                    {
                        t_batchIsExist = true;
                        break;
                    }
                }

                if( !t_batchIsExist )
                {
                    continue;
                }

                t_cardInfo.batchId = t_batchId;

                auto t_oldCardInfoIt = t_localCardList.begin();

                while( t_oldCardInfoIt != t_localCardList.end() )
                {
                    if( t_oldCardInfoIt->id.compare( t_cardInfo.id ) == 0 )
                    {
                        break;
                    }
                    t_oldCardInfoIt++;
                }

                bool t_needDownloadCoverImage = true;
                if( t_oldCardInfoIt != t_localCardList.end() )
                {
                    t_needDownloadCoverImage = false;
                    t_cardInfo.activation = t_oldCardInfoIt->activation;

                    auto t_coverFileInfo = DataTableFile::instance().findBySourceUrl( t_cardInfo.coverFileUrl );
                    if( t_coverFileInfo.sourceUrl.compare( t_coverImageUrl ) != 0 || t_coverFileInfo.fileMd5.compare( t_coverImageMd5 ) != 0 )
                    {
                        DataTableFile::instance().remove( t_coverFileInfo );
                        t_needDownloadCoverImage = !t_coverImageUrl.empty() && !t_coverImageMd5.empty();
                    }

                    DataTableCard::instance().update( t_cardInfo );

                    t_localCardList.erase( t_oldCardInfoIt );
                }else{
                    DataTableCard::instance().insert( t_cardInfo );
                }

                if( t_needDownloadCoverImage )
                {
                    //添加卡片封面到下载队列
                    UpdateDownloadItem t_downloadItem{
                        t_coverImageUrl,
                        t_coverImageMd5,
                        nullptr
                    };
                    m_downloadList.push( t_downloadItem );
                }
            }

            //删除多余的卡片

            for( int i = 0; i < t_localCardList.size(); ++i )
            {
                auto t_cardInfo = t_localCardList[i];
                DataTableFile::instance().remove( DataTableFile::instance().findBySourceUrl( t_cardInfo.coverFileUrl ) );

                auto t_cardAudioList = DataTableCardAudio::instance().list( t_cardInfo.id );

                for( int n = 0; n < t_cardAudioList.size(); ++n )
                {
                    DataTableFile::instance().remove( DataTableFile::instance().findBySourceUrl( t_cardAudioList[n].fileUrl ) );
                }

                DataTableCard::instance().remove( t_cardInfo.id );

            }

            {
                m_mutex.lock();
                checkUpdateResponse( p_http );
                m_mutex.unlock();
            }


        }, [this]( Http * p_http, std::string p_res ){

        } );
        
        m_checkCardUpdateBatchIdList[t_http] = DataCardBatchInfo::s_batchIdList[0];
        m_checkUpdateHandlerList.push_back( t_http );
        
        //更新其他卡片
        std::queue< std::function<void(void)> > t_tmpCheckUpdateQueue;
        
        while( !m_checkUpdateQueue.empty() )
        {
            t_tmpCheckUpdateQueue.push( m_checkUpdateQueue.front() );
            m_checkUpdateQueue.pop();
        }
        
        for( int i = 1; i < DataCardBatchInfo::s_batchIdList.size(); ++i )
        {
            Http::HttpParameter t_parameter;
            t_parameter["cardType"] = DataCardBatchInfo::s_batchIdList[i];
            m_checkUpdateQueue.push( [this, t_parameter](){
                auto t_pt = t_parameter;
                auto t_http = Http::Post( sm_cardListApi2, &t_pt, [this]( Http * p_http, std::string p_res ){
                    auto t_batchId = m_checkCardUpdateBatchIdList[p_http];
                    
                    auto t_cardBatchInfo = DataTableCardBatch::instance().find( t_batchId );
                    
                    auto t_localCardList = DataTableCard::instance().list( t_batchId );
                    
                    m_checkCardUpdateBatchIdList.erase( p_http );
                    
                    Document t_readdoc;
                    
                    t_readdoc.Parse<0>( p_res.c_str() );
                    
                    if( t_readdoc.HasParseError() )
                    {
                        printf( "GetParseError %d \n", t_readdoc.GetParseError() );
                    }
                    
                    auto t_success = t_readdoc["success"].GetBool();
                    if( !t_success )
                    {
                        printf( "------------> 检查更新失败" );
                        return;
                    }
                    
                    auto & t_data = t_readdoc["data"];
                    
                    //更新卡片
                    for( int i = 0; i < t_data.Capacity(); ++i )
                    {
                        auto & t_item = t_data[i];
                        
                        
                        auto t_coverImageUrl = std::string( DOMAIN_NAME ) + std::string( t_item["iconPath"].GetString() ).substr( 1 );
                        auto t_coverImageMd5 = std::string( t_item["iconMd5"].GetString() );
                        auto t_cardInfo = DataCardInfo( t_item["cardId"].GetString(),
                                                       t_item["cardType"].GetString(),
                                                       t_item["rfId"].GetInt(),
                                                       t_coverImageUrl,
                                                       t_coverImageMd5
                                                       );
                        
                        auto t_oldCardInfoIt = t_localCardList.begin();
                        while( t_oldCardInfoIt != t_localCardList.end() )
                        {
                            if( t_oldCardInfoIt->id.compare( t_cardInfo.id ) == 0 )
                            {
                                break;
                            }
                            t_oldCardInfoIt++;
                        }
                        
                        bool t_needDownloadCoverImage = true;
                        if( t_oldCardInfoIt != t_localCardList.end() )
                        {
                            t_needDownloadCoverImage = false;
                            t_cardInfo.activation = t_oldCardInfoIt->activation;
                            
                            auto t_coverFileInfo = DataTableFile::instance().findBySourceUrl( t_cardInfo.coverFileUrl );
                            if( t_coverFileInfo.sourceUrl.compare( t_coverImageUrl ) != 0 || t_coverFileInfo.fileMd5.compare( t_coverImageMd5 ) != 0 )
                            {
                                DataTableFile::instance().remove( t_coverFileInfo );
                                t_needDownloadCoverImage = true;
                            }
                            DataTableCard::instance().update( t_cardInfo );
                            
                            t_localCardList.erase( t_oldCardInfoIt );
                        }else{
                            DataTableCard::instance().insert( t_cardInfo );
                        }
                        
                        if( t_needDownloadCoverImage )
                        {
                            //添加卡片封面到下载队列
                            UpdateDownloadItem t_downloadItem{
                                t_coverImageUrl,
                                t_coverImageMd5,
                                nullptr
                            };
                            m_downloadList.push( t_downloadItem );
                        }
                        
                    }
                    
                    //删除多余的卡片
                    for( int i = 0; i < t_localCardList.size(); ++i )
                    {
                        auto t_cardInfo = t_localCardList[i];
                        DataTableFile::instance().remove( DataTableFile::instance().findBySourceUrl( t_cardInfo.coverFileUrl ) );
                        
                        auto t_cardAudioList = DataTableCardAudio::instance().list( t_cardInfo.id );
                        
                        for( int n = 0; n < t_cardAudioList.size(); ++n )
                        {
                            DataTableFile::instance().remove( DataTableFile::instance().findBySourceUrl( t_cardAudioList[n].fileUrl ) );
                        }
                        
                        DataTableCard::instance().remove( t_cardInfo.id );
                    }
                    
                    {
                        m_mutex.lock();
                        checkUpdateResponse( p_http );
                        m_mutex.unlock();
                    }
                    
                }, [this]( Http * p_http, std::string p_res ){
                    
                } );
                
                m_checkCardUpdateBatchIdList[t_http] = t_pt["cardType"];
                m_checkUpdateHandlerList.push_back( t_http );
            } );
            
        }
        
        while( !t_tmpCheckUpdateQueue.empty() )
        {
            m_checkUpdateQueue.push( t_tmpCheckUpdateQueue.front() );
            t_tmpCheckUpdateQueue.pop();
        }
    };
    
    //下载文件
    auto t_downloadFile = [this](void)
    {
        if( m_downloadList.size() )
        {
            m_messageLabel->setString( "正在下载..." );
            downloadFile();
        }
        else if( m_checkUpdateQueue.size() )
        {
            checkUpdateDequeue();
        }
    };
    
    auto loadImage = [this](void){
        m_messageLabel->setString( "加载资源..." );
        
        
        m_loadResourceCount = 0;
        
        sm_loadImageList.clear();
        sm_loadImageList.push_back( "DadGuess/DadGuessMain.png" );
        
        for( auto t_batchId : DataCardBatchInfo::s_batchIdList )
        {
            
            auto t_cardBatchInfo = DataTableCardBatch::instance().find( t_batchId );
            if( t_cardBatchInfo.activation )
            {
                DataTableCard::activation( t_cardBatchInfo.id );
            }
            
            if( s_cardList.find( t_batchId ) == s_cardList.end() )
            {
                s_cardList[t_batchId] = std::vector< std::pair< DataCardInfo, DataFileInfo > >();
            }else{
                s_cardList[t_batchId].clear();
            }
            
            auto t_cardList = DataTableCard::instance().list( t_batchId );
            for( auto t_cardInfo : t_cardList )
            {
                auto t_coverFileInfo = DataTableFile::instance().findBySourceUrl( t_cardInfo.coverFileUrl );

                auto t_pair = std::pair< DataCardInfo, DataFileInfo >( t_cardInfo, t_coverFileInfo );
                
                if( !t_coverFileInfo.fileName.empty() )
                {
                    sm_loadImageList.push_back( t_coverFileInfo.fileName );
                }
                
                s_cardList[ t_batchId ].push_back( t_pair );
            }
        }
        
        auto tf_tryStartGame = [this](){
            if( --m_loadResourceCount == 0 )
            {
                
                SpriteFrameCache::getInstance()->addSpriteFramesWithFile( "DadGuess/DadGuessMain.plist", Director::getInstance()->getTextureCache()->getTextureForKey( "DadGuess/DadGuessMain.png" ) );
                
                checkUpdateDequeue();
            }
        };
        
        m_loadResourceCount = (int)sm_loadImageList.size();
        for( size_t i = 0; i < sm_loadImageList.size(); ++i )
        {
            Director::getInstance()->getTextureCache()->addImageAsync( sm_loadImageList[i], [=]( Texture2D * p_txt ){
                tf_tryStartGame();
            });
        }
        
    };
    
    auto goMainScene = [this](void){
        m_hand->stopAllActions();
        Director::getInstance()->replaceScene( DadGuessMainScene::create() );
        destroy();

        if( m_needCehckUpdate )
        {
            DataTableKeyValue::instance().set( DataKeyValueInfo( s_checkUpdateKey, true ) );
        }
        s_updateed = true;
    };
    

    m_needCehckUpdate = true;

    if( getNetWorkState() != NetWorkStateListener::NetWorkState::WiFi && getNetWorkState() != NetWorkStateListener::NetWorkState::WWAN )
    {
        m_needCehckUpdate = false;
    }

    auto t_keyValue = DataTableKeyValue::instance().get( s_checkUpdateKey );

    if( m_needCehckUpdate && !t_keyValue.key.empty() )
    {
        time_t t_curTime;
        time(&t_curTime);
        
        if( t_curTime - t_keyValue.date < s_updateOverTime && t_keyValue.getBooleanValue() )
        {
           m_needCehckUpdate = false; 
        }
    }

    if( m_needCehckUpdate )
    {
        DataTableKeyValue::instance().set( DataKeyValueInfo( s_checkUpdateKey, false ) );
        m_checkUpdateQueue.push( t_checkCardBatchUpdate );
        m_checkUpdateQueue.push( t_checkCardUpdate );
        m_checkUpdateQueue.push( t_downloadFile );
    }
    m_checkUpdateQueue.push( loadImage );
    m_checkUpdateQueue.push( goMainScene );

    checkUpdateDequeue();
    
    schedule( schedule_selector(DadGuessUpdateScene::update) );
    
    return true;
}

void DadGuessUpdateScene::update( float p_dt )
{
    if( m_currUpdateFunc )
    {
        m_currUpdateFunc();
        m_currUpdateFunc = nullptr;
    }
}

void DadGuessUpdateScene::checkUpdateResponse( Http * p_http )
{
    m_checkUpdateHandlerList.remove( p_http );
    if( m_checkUpdateHandlerList.empty() && m_checkUpdateQueue.size() )
    {
        checkUpdateDequeue();
    }
}

void DadGuessUpdateScene::checkCardAudioUpdate( std::vector< DataCardAudioInfo > & p_oldCardAudioList, const std::vector< std::pair< std::string, std::string > > & p_seviceCardAudioList, const std::string & p_cardId, const DataCardAudioInfo::AudioType p_audioType )
{
    for( int i = 0; i < p_seviceCardAudioList.size(); ++i )
    {
        
        auto t_dataAudioUrl = p_seviceCardAudioList[i].first;
        auto t_dataAudioMd5 = p_seviceCardAudioList[i].second;
        
        auto t_audioFileInfo = DataTableFile::instance().findBySourceUrl( t_dataAudioUrl );
        auto t_serviceAudioInfo = DataCardAudioInfo( "", p_cardId, t_dataAudioUrl, t_dataAudioMd5, p_audioType );
        bool t_needDownloadAudioFile = true;
        if( !t_audioFileInfo.fileId.empty() )
        {
            //
            for( int n = 0; n < p_oldCardAudioList.size(); ++n )
            {
                if( p_oldCardAudioList[n].fileUrl.compare( t_audioFileInfo.sourceUrl ) == 0 && p_oldCardAudioList[n].fileMd5.compare( t_audioFileInfo.fileMd5 ) == 0 )
                {
                    t_needDownloadAudioFile = false;
                    p_oldCardAudioList.erase( p_oldCardAudioList.begin() + n );
                    break;
                }
            }
        }
        
        
        if( !t_needDownloadAudioFile )
        {
            DataTableCardAudio::instance().update( t_serviceAudioInfo );
        }else
        {
            //下载队列
            t_serviceAudioInfo.id = createUUID();
            m_downloadAudioList[ t_dataAudioUrl ] = t_serviceAudioInfo;
            
            UpdateDownloadItem t_downloadItem{
                t_dataAudioUrl,
                t_dataAudioMd5,
                nullptr
            };
            m_downloadList.push( t_downloadItem );
        }
    }
}

void DadGuessUpdateScene::downloadFile( void )
{
    while( m_downloadingList.size() < 6 && m_downloadList.size() )
    {
        auto t_downloadItem = m_downloadList.front();
        m_downloadList.pop();
        
        auto t_downloadHandle = Http::DownloadFile( t_downloadItem.url, "", [this]( Http * p_http, DataFileInfo p_fileInfo ){
            
            auto t_find = DataTableFile::instance().find( p_fileInfo.fileId );
            if( t_find.fileId.compare( p_fileInfo.fileId ) != 0 )
            {
                DataTableFile::instance().insert( p_fileInfo );
            }
            
            auto t_it = m_downloadingList.find( p_http );
            if( t_it == m_downloadingList.end() )
            {
                return;
            }
            
            if( t_it->second._downloadCallBack )
            {
                t_it->second._downloadCallBack( p_fileInfo );
            }
            m_downloadingList.erase( t_it );
            
            {
                m_mutex.lock();
                downloadFile();
                checkUpdateResponse( p_http );
                m_mutex.unlock();
            }
            
            
        }, [this]( Http * p_http, DataFileInfo p_fileInfo ){
            printf( "download final: %s \n", p_fileInfo.sourceUrl.c_str() );

            UpdateDownloadItem t_downloadItem{
                p_fileInfo.sourceUrl,
                "",
                nullptr
            };
            m_downloadList.push( t_downloadItem );
            
            m_mutex.lock();
            checkUpdateResponse( p_http );
            m_mutex.unlock();

        });
        
        if( t_downloadHandle )
        {
            m_checkUpdateHandlerList.push_back( t_downloadHandle );
            m_downloadingList[ t_downloadHandle ] = t_downloadItem;
        }else{
            //download file return nullptr PS: file exist
            if( t_downloadItem._downloadCallBack )
            {
                t_downloadItem._downloadCallBack( DataTableFile::instance().findBySourceUrl( t_downloadItem.url ) );
            }
        }
    }
}

void DadGuessUpdateScene::checkUpdateDequeue( void )
{
    m_currUpdateFunc = m_checkUpdateQueue.front();
    m_checkUpdateQueue.pop();
}

void DadGuessUpdateScene::unCacheResource( void )
{
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile( "DadGuess/DadGuessMain.plist" );
    
    for( int i = 0; i < sm_loadImageList.size(); ++i )
    {
        if( Director::getInstance()->getTextureCache()->getTextureForKey( sm_loadImageList[i] ) != nullptr )
        {
            Director::getInstance()->getTextureCache()->removeTextureForKey( sm_loadImageList[i] );
        }
    }
}

void DadGuessUpdateScene::destroy( void )
{
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile( "DadGuess/DadGuessUpdate.plist" );
    Director::getInstance()->getTextureCache()->removeTextureForKey( "DadGuess/DadGuessUpdate.png" );
}
