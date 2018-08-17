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

USING_NS_CC;
using namespace network;


network::Downloader * Http::sm_downloader = nullptr;
std::map< const std::string, Http * > Http::sm_downloadTaskList;

std::string Http::sm_writePath = FileUtils::getInstance()->getWritablePath();

void Http::Get( const std::string & p_url, HttpParameter * p_parameter, HttpCallBack p_success, HttpCallBack p_final )
{
    Http * t_http = new Http;

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

        t_url << parseParameter( p_parameter );

        t_requerstUrl = t_url.str();
    }

    //    生成HttpRequest对象
    auto request = new HttpRequest();
    //    设置请求
    request->setUrl( t_requerstUrl.c_str() );
    //    设置请求方式  GET类型
    request->setRequestType( HttpRequest::Type::GET );
    //    设置请求完成后的回调函数
    
    request->setResponseCallback( CC_CALLBACK_2( Http::getHttp_handshakeResponse, t_http) );
    //    设置请求tag
    request->setTag("GET");
    //    生成HttpClient对象，并且发送请求
    HttpClient::getInstance()->send( request );
    //    释放HttpRequest对象
    request->release();

}

void Http::Post( const std::string & p_url, HttpParameter * p_parameter, HttpCallBack p_success, HttpCallBack p_final )
{
    Http * t_http = new Http;

    t_http->m_success = p_success;
    t_http->m_final = p_final;

    //    生成HttpRequest对象
    auto request = new HttpRequest();
    //    设置请求
    request->setUrl( p_url.c_str() );
    //    设置请求方式  POST类型
    request->setRequestType( HttpRequest::Type::POST );
    //    设置请求完成后的回调函数
    
    request->setResponseCallback( CC_CALLBACK_2( Http::getHttp_handshakeResponse, t_http ) );

    std::vector<std::string> headers;
    headers.push_back("Content-Type: application/x-www-form-urlencoded; charset=UTF-8");
    // 设置请求头，如果数据为键值对则不需要设置

    request -> setHeaders( headers );
    // 传入发送的数据及数据
    std::string t_data = parseParameter( p_parameter );
    request -> setRequestData(t_data.c_str(), t_data.size());

    //    设置请求tag
    request->setTag("POST");
    //    生成HttpClient对象，并且发送请求
    HttpClient::getInstance()->send( request );
    //    释放HttpRequest对象
    request->release();
}

void Http::DownloadFile( const std::string & p_url, const std::string & p_filePath, DownloadFileCallBack p_success, DownloadFileCallBack p_final, DownloadFileProgressCallBack p_progress )
{

    Http * t_http = new Http();

    t_http->m_downloadSuccessCallBack = p_success;
    t_http->m_downloadFinalCallBack = p_final;
    t_http->m_downloadProgressCallBack = p_progress;

    if( sm_downloader == nullptr )
    {
        sm_downloader = new network::Downloader();

        auto & t_downloadTaskList = sm_downloadTaskList;

        sm_downloader->setOnFileTaskSuccess( [&t_downloadTaskList]( const network::DownloadTask & p_downloadTask ){
            printf( "OnFileTaskSuccess \n" );

            auto t_currTask = t_downloadTaskList.find( p_downloadTask.identifier );
            
            if( t_currTask == t_downloadTaskList.end() )
            {
                return;
            }
            
            Http * p_http = t_currTask->second;

            if( p_http->getDownloadSuccessCallBack() )
            {
                p_http->getDownloadSuccessCallBack()( p_downloadTask.requestURL, p_downloadTask.storagePath );
            }

            t_downloadTaskList.erase( p_downloadTask.identifier );
            delete p_http;
        } );

        sm_downloader->setOnTaskError( [&t_downloadTaskList]( const network::DownloadTask & p_downloadTask, int p_errorCode, int p_errorCodeInternal, const std::string & p_errorStr ){
            printf( "OnTaskError{ p_errorCode: %d, p_errorCodeInternal: %d, p_errorStr: %s } \n", p_errorCode, p_errorCodeInternal, p_errorStr.c_str() );

            auto t_currTask = t_downloadTaskList.find( p_downloadTask.identifier );
            
            if( t_currTask == t_downloadTaskList.end() )
            {
                return;
            }
            
            Http * p_http = t_currTask->second;

            if( p_http->getDownloadFinalCallBack() )
            {
                p_http->getDownloadFinalCallBack()( p_downloadTask.requestURL, p_downloadTask.storagePath );
            }
            
            t_downloadTaskList.erase( p_downloadTask.identifier );
            delete p_http;
        } );

        sm_downloader->setOnTaskProgress( [&t_downloadTaskList]( const network::DownloadTask & p_downloadTask, long p_bytesReceived, long p_totalBytesReceived, long p_totalBytesExpected ){
            printf( "OnTaskProgress{ p_bytesReceived: %ld, p_totalBytesReceived: %ld, p_totalBytesExpected: %ld } \n", p_bytesReceived, p_totalBytesReceived, p_totalBytesExpected );

            auto t_currTask = t_downloadTaskList.find( p_downloadTask.identifier );
            
            if( t_currTask == t_downloadTaskList.end() )
            {
                return;
            }
            
            Http * p_http = t_currTask->second;

            if( p_http->getDownloadProgressCallBack() )
            {
                p_http->getDownloadProgressCallBack()( p_downloadTask.requestURL, p_downloadTask.storagePath, p_totalBytesReceived, p_totalBytesExpected );
            }
            
        } );

    }

    std::string t_taskUUID = createUUID();
    auto t_downloadTask = sm_downloader->createDownloadFileTask( p_url, sm_writePath + p_filePath, t_taskUUID );

    sm_downloadTaskList[ t_taskUUID ] = t_http;

}

void Http::getHttp_handshakeResponse( network::HttpClient * p_sender, network::HttpResponse * p_response )
{
    
    std::vector<char> * t_hander = p_response->getResponseHeader();
    
    if( t_hander->size() == 0 )
    {
        m_final( "network not ready" );
        delete this;
        return; //
    }
    std::vector<char> * t_response = p_response->getResponseData();
    std::string t_strHander = std::string( &*t_hander->begin() );

    printf( "t_strHander: %s \n", t_strHander.c_str() );

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

    
//    std::string t_strResponse = std::string( &*t_response->begin(), t_contentLength );

    std::string t_strResponse;
    t_strResponse.insert( t_strResponse.begin(), t_response->begin(), t_response->end() );

    if( t_httpState != 200 )
    {
        m_final( t_strResponse );
    }else{
        m_success( t_strResponse );
    }
    
    delete this;
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
