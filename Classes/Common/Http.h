//
//  Http.hpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/7/10.
//

#ifndef __HTTP_H__
#define __HTTP_H__

#include "network/HttpClient.h"
#include <string>
#include <functional>
#include <map>
#include "cocos/network/CCDownloader.h"
#include "DataTableFile.h"

class Http
{
public:

    typedef std::function<void( Http * p_http, std::string )> HttpCallBack;
    typedef std::map< std::string, std::string > HttpParameter;
    typedef std::function<void( Http * p_http, DataFileInfo p_fileInfo )> DownloadFileCallBack;
    typedef std::function<void( Http * p_http, DataFileInfo p_fileInfo, long, long )> DownloadFileProgressCallBack;

    static std::string token;

    static Http * Get( const std::string & p_url, HttpParameter * p_parameter, HttpCallBack p_success, HttpCallBack p_final );
    static Http * Post( const std::string & p_url, HttpParameter * p_parameter, HttpCallBack p_success, HttpCallBack p_final );

    static Http * DownloadFile( const std::string & p_url, const std::string & p_fileSuffixName, DownloadFileCallBack p_success, DownloadFileCallBack p_final, DownloadFileProgressCallBack p_progress = nullptr );
    
    void http_handshakeResponse( cocos2d::network::HttpClient *p_sender, cocos2d::network::HttpResponse *p_response );

    DownloadFileCallBack getDownloadSuccessCallBack( void );
    DownloadFileCallBack getDownloadFinalCallBack( void );
    DownloadFileProgressCallBack getDownloadProgressCallBack( void );

private:

    static std::string parseParameter( HttpParameter * p_parameter );
    static DataFileInfo convertToFileInfo( const cocos2d::network::DownloadTask & p_downloadTask );
    static std::map< const std::string, Http * > sm_downloadTaskList;

    HttpCallBack m_success, m_final;
    DownloadFileCallBack m_downloadSuccessCallBack, m_downloadFinalCallBack;
    DownloadFileProgressCallBack m_downloadProgressCallBack;

    static cocos2d::network::Downloader * sm_downloader;
};

#endif /* __HTTP_H__ */
