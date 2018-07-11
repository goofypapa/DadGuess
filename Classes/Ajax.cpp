//
//  Ajax.cpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/7/10.
//

#include "Ajax.h"
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include "Common.h"
#include <vector>

USING_NS_CC;
using namespace network;


void Ajax::Get( const std::string & p_url, AjaxParameter * p_parameter, AjaxCallBack p_success, AjaxCallBack p_final )
{
    Ajax * t_ajax = new Ajax;

    t_ajax->m_success = p_success;
    t_ajax->m_final = p_final;

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

        int t_index = 0;
        for( auto item : (*p_parameter) )
        {

            t_url << item.first << "=" << item.second;
            
            if( ++t_index != p_parameter->size() )
            {
                t_url << "&";
            }
        }

        t_requerstUrl = t_url.str();
    }

    printf( "t_requerstUrl: %s \n", t_requerstUrl.c_str() );

    //    生成HttpRequest对象
    auto request = new HttpRequest();
    //    设置请求
    request->setUrl( t_requerstUrl.c_str() );
    //    设置请求方式  GET类型
    request->setRequestType( HttpRequest::Type::GET );
    //    设置请求完成后的回调函数
    
    printf( "obj: %d \n", t_ajax );
    request->setResponseCallback( CC_CALLBACK_2( Ajax::getHttp_handshakeResponse, t_ajax) );
    //    设置请求tag
    request->setTag("GET");
    //    生成HttpClient对象，并且发送请求
    HttpClient::getInstance()->send( request );
    //    释放HttpRequest对象
    request->release();

}

void Ajax::Post( const std::string & p_url, const std::string & p_data, AjaxCallBack p_success, AjaxCallBack p_final )
{
    Ajax * t_ajax = new Ajax;

    t_ajax->m_success = p_success;
    t_ajax->m_final = p_final;

    //    生成HttpRequest对象
    auto request = new HttpRequest();
    //    设置请求
    request->setUrl( p_url.c_str() );
    //    设置请求方式  POST类型
    request->setRequestType( HttpRequest::Type::POST );
    //    设置请求完成后的回调函数
    
    request->setResponseCallback( CC_CALLBACK_2( Ajax::getHttp_handshakeResponse, t_ajax) );

    std::vector<std::string> headers;
    headers.push_back("Content-Type: application/json; charset=utf-8");
    // 设置请求头，如果数据为键值对则不需要设置

    request -> setHeaders( headers );
    // 传入发送的数据及数据
    request -> setRequestData(p_data.c_str(), p_data.size());

    //    设置请求tag
    request->setTag("POST");
    //    生成HttpClient对象，并且发送请求
    HttpClient::getInstance()->send( request );
    //    释放HttpRequest对象
    request->release();
}

void Ajax::getHttp_handshakeResponse( network::HttpClient * p_sender, network::HttpResponse * p_response )
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
            t_httpState = atoi( t_httpStateList[1].c_str() );
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
    
    std::string t_strResponse = std::string( &*t_response->begin(), t_contentLength );
    
    if( t_httpState != 200 )
    {
        m_final( t_strResponse );
    }else{
        m_success( t_strResponse );
    }
    
    delete this;
}
