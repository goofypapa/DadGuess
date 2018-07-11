//
//  Ajax.hpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/7/10.
//

#ifndef __AJAX_H__
#define __AJAX_H__

#include "network/HttpClient.h"
#include <string>
#include <functional>
#include <map>

class Ajax
{
public:

    typedef std::function<void( std::string )> AjaxCallBack;
    typedef std::map< std::string, std::string > AjaxParameter;

    static void Get( const std::string & p_url, AjaxParameter * p_parameter, AjaxCallBack p_success, AjaxCallBack p_final );
    static void Post( const std::string & p_url, const std::string & p_data, AjaxCallBack p_success, AjaxCallBack p_final );
    
    void getHttp_handshakeResponse( cocos2d::network::HttpClient *p_sender, cocos2d::network::HttpResponse *p_response );

private:
    AjaxCallBack m_success, m_final;
};

#endif /* __AJAX_H__ */
