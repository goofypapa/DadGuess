//
//  Http.cpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/7/10.
//

#include "Http.h"
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include "Common.h"
#include <vector>
#include "md5.hpp"
#include "BaseScene.h"
#include "DataTableWebServiceDataCache.h"
#include <thread>

USING_NS_CC;
using namespace network;


network::Downloader * Http::sm_downloader = nullptr;
std::map< const std::string, Http * > Http::sm_downloadTaskList;
std::map< Http *, std::string > Http::sm_cacheKeyList;

std::string Http::token = "";

const int Http::sm_overtime = 60 * 60 * 24 * 1;

Http * Http::Get( const std::string & p_url, HttpParameter * p_parameter, HttpCallBack p_success, HttpCallBack p_final, const bool p_enableCache )
{
    std::string t_data = p_parameter ? parseParameter( p_parameter ) : "" ;
    std::string t_cacheKey = strToSqlStr( p_url + t_data );

    if( p_enableCache )
    {
        time_t t_curTime;
        time(&t_curTime);
        auto t_dataCacheInfo = DataTableWebServiceDataCache::instance().find( t_cacheKey );
        if( !t_dataCacheInfo.id.empty() && ( t_curTime - t_dataCacheInfo.date < sm_overtime || ( getNetWorkState() != NetWorkStateListener::NetWorkState::WiFi && getNetWorkState() != NetWorkStateListener::NetWorkState::WWAN ) ) )
        {
            Http * t_http = new Http;
            std::thread( []( HttpCallBack p_callBack, Http * p_id, const std::string & p_res ){
                p_callBack( p_id, sqlStrToStr(  p_res ) );
                delete p_id;
            }, p_success, t_http, t_dataCacheInfo.res ).detach();
            return t_http;
        }
    }

    Http * t_http = new Http;
    sm_cacheKeyList[ t_http ] = t_cacheKey;

    t_http->m_success = p_success;
    t_http->m_final = p_final;

    std::string t_requerstUrl = p_url;

    if( p_parameter != nullptr )
    {
        std::stringstream t_url;
        auto t_findRest = p_url.find( "?" );
        if( t_findRest == std::string::npos )
        {
            t_url << p_url << "?";
        }else{
            t_url << p_url << "&";
        }

        t_url << t_data;

        t_requerstUrl = t_url.str();
    }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    httpGet( t_requerstUrl, token, createUUID(), [t_http]( std::string p_requestId, std::string p_res ){
        t_http->getSuccessCallBack()( t_http, p_res );
    }, [t_http]( std::string p_requestId, std::string p_res ){
        t_http->getFinalCallBack()( t_http, p_res );
    } );
#else

    //    生成HttpRequest对象
    auto request = new HttpRequest();

    std::vector<std::string> headers;
    if( !token.empty() )
    {
        headers.push_back( std::string( "Authorization: " ) + token );
    }
    
    request -> setHeaders( headers );
    //    设置请求
    request->setUrl( t_requerstUrl.c_str() );
    //    设置请求方式  GET类型
    request->setRequestType( HttpRequest::Type::GET );
    //    设置请求完成后的回调函数
    
    request->setResponseCallback( CC_CALLBACK_2( Http::http_handshakeResponse, t_http) );
    //    设置请求tag
    request->setTag("GET");
    //    生成HttpClient对象，并且发送请求
    HttpClient::getInstance()->sendImmediate( request );
    //    释放HttpRequest对象
    request->release();
#endif

    return t_http;
}

Http * Http::Post( const std::string & p_url, HttpParameter * p_parameter, HttpCallBack p_success, HttpCallBack p_final, const bool p_enableCache )
{
    std::string t_data = p_parameter ? parseParameter( p_parameter ) : "" ;
    std::string t_cacheKey = strToSqlStr( p_url + t_data );
    
    if( p_enableCache )
    {
        time_t t_curTime;
        time(&t_curTime);
        auto t_dataCacheInfo = DataTableWebServiceDataCache::instance().find( t_cacheKey );
        if( !t_dataCacheInfo.id.empty() && ( t_curTime - t_dataCacheInfo.date < sm_overtime || ( getNetWorkState() != NetWorkStateListener::NetWorkState::WiFi && getNetWorkState() != NetWorkStateListener::NetWorkState::WWAN ) ) )
        {
            Http * t_http = new Http;
            std::thread( []( HttpCallBack p_callBack, Http * p_id, const std::string & p_res ){
                p_callBack( p_id, sqlStrToStr(  p_res ) );
                delete p_id;
            }, p_success, t_http, t_dataCacheInfo.res ).detach();
            return t_http;
        }
    }

    Http * t_http = new Http;
    sm_cacheKeyList[ t_http ] = t_cacheKey;

    t_http->m_success = p_success;
    t_http->m_final = p_final;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    httpPost( p_url, t_data, token, createUUID(), [t_http]( std::string p_requestId, std::string p_res ){
        t_http->getSuccessCallBack()( t_http, p_res );
    }, [t_http]( std::string p_requestId, std::string p_res ){
        t_http->getFinalCallBack()( t_http, p_res );
    } );
#else

   //    生成HttpRequest对象
   auto request = new HttpRequest();
   //    设置请求
   request->setUrl( p_url.c_str() );
   //    设置请求方式  POST类型
   request->setRequestType( HttpRequest::Type::POST );
   //    设置请求完成后的回调函数

   request->setResponseCallback( CC_CALLBACK_2( Http::http_handshakeResponse, t_http ) );

   std::vector<std::string> headers;
   headers.push_back("Content-Type: application/x-www-form-urlencoded; charset=UTF-8");
   // 设置请求头，如果数据为键值对则不需要设置

   if( !token.empty() )
   {
       headers.push_back( std::string( "Authorization: " ) + token );
   }

   request -> setHeaders( headers );
   // 传入发送的数据及数据
   request -> setRequestData(t_data.c_str(), t_data.size());

   //    设置请求tag
   request->setTag("POST");
   //    生成HttpClient对象，并且发送请求
   HttpClient::getInstance()->send( request );
   //    释放HttpRequest对象
   request->release();
#endif

    return t_http;
}

Http *  Http::DownloadFile( const std::string & p_url, const std::string & p_fileSuffixName, DownloadFileCallBack p_success, DownloadFileCallBack p_final, DownloadFileProgressCallBack p_progress )
{

    auto t_fileInfo = DataTableFile::instance().findBySourceUrl( p_url );

    if( !t_fileInfo.fileId.empty() && FileUtils::getInstance()->isFileExist( fullFilePath( t_fileInfo.fileName ) ) )
    {
        p_success( nullptr, t_fileInfo );
        return nullptr;
    }


    Http * t_http = new Http();

    t_http->m_downloadSuccessCallBack = p_success;
    t_http->m_downloadFinalCallBack = p_final;
    t_http->m_downloadProgressCallBack = p_progress;

    if( sm_downloader == nullptr )
    {
        sm_downloader = new network::Downloader();

        auto & t_downloadTaskList = sm_downloadTaskList;

        sm_downloader->setOnFileTaskSuccess( [&t_downloadTaskList]( const network::DownloadTask & p_downloadTask ){

            auto t_currTask = t_downloadTaskList.find( p_downloadTask.identifier );
            
            if( t_currTask == t_downloadTaskList.end() )
            {
                return;
            }

            Http * p_http = t_currTask->second;

            DataFileInfo t_fileInfo = convertToFileInfo( p_downloadTask );
            
            printf( "OnFileTaskSuccess: %s \n", t_fileInfo.toJson().c_str() );

            if( DataTableFile::instance().insert( t_fileInfo ) )
            {
                if( p_http->getDownloadSuccessCallBack() )
                {
                    p_http->getDownloadSuccessCallBack()( sm_downloadTaskList[p_downloadTask.identifier], t_fileInfo );
                }
            }else{
                if( p_http->getDownloadFinalCallBack() )
                {
                    p_http->getDownloadFinalCallBack()( sm_downloadTaskList[p_downloadTask.identifier], t_fileInfo );
                }
            }

            auto t_activityScene = BaseScene::activityScene();
            if( t_activityScene )
            {
                t_activityScene->refreshSource( t_fileInfo );
            }

            t_downloadTaskList.erase( p_downloadTask.identifier );
            delete p_http;
        } );

        sm_downloader->setOnTaskError( [&t_downloadTaskList]( const network::DownloadTask & p_downloadTask, int p_errorCode, int p_errorCodeInternal, const std::string & p_errorStr ){
//             printf( "OnTaskError{ p_errorCode: %d, p_errorCodeInternal: %d, p_errorStr: %s } \n", p_errorCode, p_errorCodeInternal, p_errorStr.c_str() );

            auto t_currTask = t_downloadTaskList.find( p_downloadTask.identifier );
            
            if( t_currTask == t_downloadTaskList.end() )
            {
                return;
            }
            
            Http * p_http = t_currTask->second;

            DataFileInfo t_fileInfo = convertToFileInfo( p_downloadTask );

            if( p_http->getDownloadFinalCallBack() )
            {
                p_http->getDownloadFinalCallBack()( sm_downloadTaskList[p_downloadTask.identifier], t_fileInfo );
            }
            
            t_downloadTaskList.erase( p_downloadTask.identifier );
            delete p_http;
        } );

        sm_downloader->setOnTaskProgress( [&t_downloadTaskList]( const network::DownloadTask & p_downloadTask, long p_bytesReceived, long p_totalBytesReceived, long p_totalBytesExpected ){
//             printf( "OnTaskProgress{ p_bytesReceived: %ld, p_totalBytesReceived: %ld, p_totalBytesExpected: %ld } \n", p_bytesReceived, p_totalBytesReceived, p_totalBytesExpected );

            auto t_currTask = t_downloadTaskList.find( p_downloadTask.identifier );
            
            if( t_currTask == t_downloadTaskList.end() )
            {
                return;
            }
            
            Http * p_http = t_currTask->second;

            DataFileInfo t_fileInfo = convertToFileInfo( p_downloadTask );

            if( p_http->getDownloadProgressCallBack() )
            {
                p_http->getDownloadProgressCallBack()( sm_downloadTaskList[p_downloadTask.identifier], t_fileInfo, p_totalBytesReceived, p_totalBytesExpected );
            }
            
        } );

    }

    std::string t_taskUUID = createUUID();
    std::string t_fileSuffixName = p_fileSuffixName;
    
    if( t_fileSuffixName.empty() )
    {
        auto t_tmp = split( p_url, "." );
        t_fileSuffixName = t_tmp[t_tmp.size() - 1];
    }
    
    auto t_downloadTask = sm_downloader->createDownloadFileTask( p_url, fullFilePath( t_taskUUID + "." + t_fileSuffixName ), t_taskUUID );

    sm_downloadTaskList[ t_taskUUID ] = t_http;

    return t_http;
}

void Http::http_handshakeResponse( network::HttpClient * p_sender, network::HttpResponse * p_response )
{
    std::vector<char> * t_hander = p_response->getResponseHeader();
    
    if( t_hander->size() == 0 )
    {
        m_final( this, "network not ready" );
        delete this;
        return; //
    }
    std::vector<char> * t_response = p_response->getResponseData();
    std::string t_strHander = std::string( &*t_hander->begin() );

    // printf( "t_strHander: %s \n", t_strHander.c_str() );

    std::vector<std::string> t_handerList = split( t_strHander, "\n" );
    
    int t_httpState, t_contentLength;
    std::string t_contentType;
    
    for( auto t_item : t_handerList )
    {
        auto t_list = split( t_item, ": " );
        if( t_list.size() == 1 )
        {
            //解析头
            auto t_httpStateList = split( t_list[0], " " );

            if( t_httpStateList[0] == "HTTP/1.1" )
            {
                t_httpState = atoi( t_httpStateList[1].c_str() );
            }


        }else if( t_list.size() == 2 )
        {
            if( t_list[0] == "Content-Type"  )
            {
                
            }
            
            if( t_list[0] == "Content-Length"  )
            {
                t_contentLength = atoi( t_list[1].c_str() );
            }
            
            if( t_list[0] == "Server"  )
            {
                
            }
            
            if( t_list[0] == "Date"  )
            {
                
            }
        }
    }

    std::string t_strResponse;
    t_strResponse.insert( t_strResponse.begin(), t_response->begin(), t_response->end() );

    if( t_httpState != 200 )
    {
        m_final( this, t_strResponse );
    }else{

        if( sm_cacheKeyList.find( this ) != sm_cacheKeyList.end() )
        {
            time_t t_curTime;
            time(&t_curTime);
            
            auto t_url = sm_cacheKeyList[this];
            auto t_cacheInfo = DataWebServiceDataCacheInfo( createUUID(), t_url, strToSqlStr( t_strResponse ), t_curTime );
            auto t_oldCacheInfo = DataTableWebServiceDataCache::instance().find( sm_cacheKeyList[this] );

            if( t_oldCacheInfo.id.empty() )
            {
                DataTableWebServiceDataCache::instance().insert( t_cacheInfo );
            }else{
                DataTableWebServiceDataCache::instance().update( t_cacheInfo );
            }

            sm_cacheKeyList.erase( this );
        }

        m_success( this, t_strResponse );
    }
    
    delete this;
}

Http::HttpCallBack Http::getSuccessCallBack( void )
{
    return m_success;
}
Http::HttpCallBack Http::getFinalCallBack( void )
{
    return m_final;
}

Http::DownloadFileCallBack Http::getDownloadSuccessCallBack( void )
{
    return m_downloadSuccessCallBack;
}

Http::DownloadFileCallBack Http::getDownloadFinalCallBack( void )
{
    return m_downloadFinalCallBack;
}

Http::DownloadFileProgressCallBack Http::getDownloadProgressCallBack( void )
{
    return m_downloadProgressCallBack;
}

std::string Http::parseParameter( HttpParameter * p_parameter )
{
    std::stringstream sstr;
    int t_index = 0;
    for( auto item : (*p_parameter) )
    {

        sstr << item.first << "=" << item.second;
        
        if( ++t_index != p_parameter->size() )
        {
            sstr << "&";
        }
    }

    return sstr.str();
}

DataFileInfo Http::convertToFileInfo( const cocos2d::network::DownloadTask & p_downloadTask )
{ 

    auto t_filePath = split( p_downloadTask.storagePath, "/" );

    DataFileInfo t_fileInfo;
    t_fileInfo.fileId = p_downloadTask.identifier;
    t_fileInfo.sourceUrl = p_downloadTask.requestURL;
    t_fileInfo.fileName = t_filePath[ t_filePath.size() - 1 ];
    t_fileInfo.fileMd5 = "";

    if( FileUtils::getInstance()->isFileExist( p_downloadTask.storagePath ) )
    {
        //计算文件md5
        auto t_if = std::ifstream( p_downloadTask.storagePath, std::ios::in );
        t_fileInfo.fileMd5 = WS_TOOLS::md5( t_if ).digest();
    }

    return t_fileInfo;
}
