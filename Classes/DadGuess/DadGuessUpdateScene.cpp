//
//  DadGuessUpdateScene.cpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/10/29.
//

#include "DadGuessUpdateScene.h"
#include "DadGuess.hpp"
#include "Common.h"

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

#include "DataTableFile.h"
#include "DataTableCardBatch.h"

USING_NS_CC;
using namespace rapidjson;

#define DOMAIN_NAME "http://www.dadpat.com/"

const char * DadGuessUpdateScene::sm_batchListApi = DOMAIN_NAME "resource/batch/list/summary.do";
const char * DadGuessUpdateScene::sm_cardListApi = DOMAIN_NAME "api/card/res/list.do";

cocos2d::Scene * DadGuessUpdateScene::CreateScene( void )
{
    return create();
}

bool DadGuessUpdateScene::init( void )
{
    if( !BaseScene::init() )
    {
        return false;
    }
    
    TexturePacker::DadGuess::addSpriteFramesToCache();
    
    auto t_visibleSizeHalf = Director::getInstance()->getVisibleSize() * 0.5f;
    Vec2 t_origin = Director::getInstance()->getVisibleOrigin();
    
    auto t_centerPos = Vec2( t_origin.x + t_visibleSizeHalf.width, t_origin.y + t_visibleSizeHalf.height );
    
    DrawNode * t_backgroundColor = DrawNode::create();
    
    t_backgroundColor->setPosition( Vec2( t_origin.x + t_visibleSizeHalf.width, t_origin.y + t_visibleSizeHalf.height ) );
    
    t_backgroundColor->drawSolidRect( Vec2( -t_visibleSizeHalf.width, -t_visibleSizeHalf.height ), Vec2( t_visibleSizeHalf.width, t_visibleSizeHalf.height ), Color4F( 122.0f / 255.0f, 212.0f / 255.0f, 249.0f / 255.0f, 1.0f ) );
    
    addChild( t_backgroundColor );
    
    auto t_head = TexturePacker::DadGuess::createHeadSprite();
    t_head->setScale( adaptation() );
    auto t_headSizeHalf = t_head->getContentSize() * 0.5f;
    auto t_headPos = t_centerPos + Vec2( 0.0f, t_visibleSizeHalf.height * 0.2f );
    t_head->setPosition( t_headPos );
    addChild( t_head );
    
    auto t_dot = TexturePacker::DadGuess::createDotSprite();
    float t_dotDistance = t_headSizeHalf.width * 1.5f;
    t_dot->setPosition( Vec2( t_headSizeHalf.width, t_headSizeHalf.height ) );
    t_head->addChild( t_dot );
    
    auto  t_eyeDistance = t_headSizeHalf.width * 0.07f;
    auto t_eyeLeft = TexturePacker::DadGuess::createEyeSprite();
    auto t_eyeLeftPos = Vec2( t_headSizeHalf.width * 0.73f, t_headSizeHalf.height * 0.885f );
    t_eyeLeft->setPosition( t_eyeLeftPos );
    t_head->addChild( t_eyeLeft );
    
    auto t_eyeRight = TexturePacker::DadGuess::createEyeSprite();
    auto t_eyeRightPos = Vec2( t_headSizeHalf.width * 1.27f, t_headSizeHalf.height * 0.885f );
    t_eyeRight->setPosition( t_eyeRightPos );
    t_head->addChild( t_eyeRight );
    
    
    t_head->runAction( RepeatForever::create( ActionFloat::create( 3.0f , 0.0f, 1.0f, [=]( const float t_dt ){
        float t_seed = t_dt * PI * -2.0f;
        t_dot->setPosition( Vec2( t_headSizeHalf.width + cos( t_seed ) * t_dotDistance, t_headSizeHalf.height + sin( t_seed ) * t_dotDistance ) );
        t_dot->setRotation( t_dt * 360.0f );
        
        auto t_eyeOffset = Vec2( cos( t_seed ) * t_eyeDistance, sin( t_seed ) * t_eyeDistance );
        t_eyeLeft->setPosition( t_eyeLeftPos + t_eyeOffset );
        t_eyeRight->setPosition( t_eyeRightPos + t_eyeOffset );
        
    }) ) );
    
    
    //更新内容提示
    auto t_message = Label::createWithSystemFont( "正在检查更新..." , "", 12 );
    t_message->setPosition( t_headPos - Vec2( 0.0f, t_dotDistance * adaptation() * 2.0f ) );
    addChild( t_message );
    
    
    //检查更新
    
    //更新批次
    auto t_checkCardBatchUpdate = [this]( void ){
        Http::HttpParameter t_parameter;
        m_checkUpdateHandlerList.push_back( Http::Post( sm_batchListApi , &t_parameter, [this]( Http * p_http, std::string p_res ){
            
            auto t_localCardBatchList = DataTableCardBatch::instance().list();
            
            Document t_readdoc;
            
            t_readdoc.Parse<0>( p_res.c_str() );
            
            if( t_readdoc.HasParseError() )
            {
                printf( "GetParseError %d \n", t_readdoc.GetParseError() );
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
                                                         t_item["batchSource"].GetString(),
                                                         t_item["batchDesc"].GetString(),
                                                         t_item["coverImage"].GetString(),
                                                         t_item["coverMd5"].GetString()
                                                         );
                
                bool t_isExit = false;
                DataCardBatchInfo t_localCardBatchInfo;
                for( auto t_item = t_localCardBatchList.begin(); t_item != t_localCardBatchList.end(); ++t_item )
                {
                    if( t_item->id.compare( t_serviceCardBatchInfo.id ) == 0 )
                    {
                        t_isExit = true;
                        
                        t_localCardBatchInfo = *t_item;
                        
                        t_localCardBatchList.erase( t_item );
                        break;
                    }
                }
                
                if( t_isExit )
                {
                    DataFileInfo t_oldCoverFile = DataTableFile::instance().findBySourceUrl( t_localCardBatchInfo.cover );
                    if( t_localCardBatchInfo.cover.compare( t_serviceCardBatchInfo.cover ) != 0 )
                    {
                        DataTableFile::instance().remove( t_oldCoverFile );
                    }
                    DataTableCardBatch::instance().update( t_serviceCardBatchInfo );
                }else
                {
                    DataTableCardBatch::instance().insert( t_serviceCardBatchInfo );
                }
                
                DataFileInfo t_coverFileInfo = DataTableFile::instance().findBySourceUrl( t_serviceCardBatchInfo.cover );
                if( t_coverFileInfo.sourceUrl.compare( t_serviceCardBatchInfo.cover ) != 0 || t_coverFileInfo.fileMd5.compare( t_serviceCardBatchInfo.coverMd5 ) != 0 )
                {
                    //添加到下载队列
                }
                
            }
            
            for( int i = 0; i < t_localCardBatchList.size(); ++i )
            {
                DataTableCardBatch::instance().remove( t_localCardBatchList[i].id );
            }
            
            checkUpdateResponse( p_http );
        }, []( Http * p_http, std::string p_res ){
            
        } ) );
    };
    
    
    //更新卡片
    auto t_checkCardUpdate = [this](){
        
        //更新动物卡片
        auto t_localCardBatchList = DataTableCardBatch::instance().list();
        for( int i = 0; i < t_localCardBatchList.size(); ++i )
        {
            Http::HttpParameter t_parameter;
            m_checkUpdateHandlerList.push_back( Http::Post( sm_cardListApi, &t_parameter, [this]( Http * p_http, std::string p_res ){
                
                
                
            }, [this]( Http * p_http, std::string p_res ){
                
            } ) );
        }
        
        //更新其他卡片

    };
    
    m_checkUpdateQueue.push( t_checkCardBatchUpdate );
    m_checkUpdateQueue.push( t_checkCardUpdate );
    
    m_checkUpdateQueue.front()();
    m_checkUpdateQueue.pop();
    
    return true;
}

void DadGuessUpdateScene::checkUpdateResponse( Http * p_http )
{
    m_checkUpdateHandlerList.remove( p_http );
    if( !m_checkUpdateHandlerList.size() )
    {
        m_checkUpdateQueue.front()();
        m_checkUpdateQueue.pop();
    }
}

DadGuessUpdateScene::~DadGuessUpdateScene( void )
{
    TexturePacker::DadGuess::removeSpriteFramesFromCache();
}
