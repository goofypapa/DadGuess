//
//  DataValidate.hpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/7/18.
//

#ifndef __DATA_VALIDATE_H__
#define __DATA_VALIDATE_H__

#include <string>
#include <regex>
#include "external/json/document.h"

inline bool IsPhoneNumber( const std::string & p_str )
{
    printf( "validate: %s \n", p_str.c_str() );
    std::regex t_regex( "^(\\d{2})?1\\d{10}$" );
    if( std::regex_match( p_str.begin(), p_str.end(), t_regex ) )
    {
        return true;
    }
    
    return false;
}

inline bool ParseJson( rapidjson::Document & p_rst, const std::string & p_str )
{
    p_rst.Parse( p_str.c_str() );
    
    if( p_rst.HasParseError() )
    {
        return false;
    }
    
    if( !p_rst.IsObject() )
    {
        return false;
    }
    
    return true;
}

inline bool ParseApiResult( rapidjson::Document & p_rst, const std::string & p_str )
{
    if( !ParseJson( p_rst, p_str ) )
    {
        return false;
    }
    
    if( !p_rst.HasMember( "success" ) )
    {
        return false;
    }
    
    return true;
}

#endif /* __DATA_VALIDATE_H__ */
