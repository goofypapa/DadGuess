//
//  Common.cpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/7/6.
//

#include "Common.h"


float adaptation( Vec2 p_sourcePixel )
{
    
    static Vec2 t_phonePixel = Director::getInstance()->getOpenGLView()->getFrameSize();
    static float t_result = MIN( t_phonePixel.x / p_sourcePixel.x , t_phonePixel.y / p_sourcePixel.y );
    
    return t_result;
}

std::vector<std::string> split( const std::string & p_str, const std::string & p_clapboard )
{
    std::vector<std::string> t_result;
    std::string::size_type t_posStart, t_posEnd;

    t_posStart = 0;
    while(1)
    {
        t_posEnd = p_str.find( p_clapboard, t_posStart );
        
        if( std::string::npos == t_posEnd )
        {
            t_result.push_back( p_str.substr( t_posStart ) );
            break;
        }

        t_result.push_back( p_str.substr( t_posStart, t_posEnd - t_posStart ) );
        t_posStart = t_posEnd + p_clapboard.size();
    }

    return t_result;
}

std::vector<std::string> split( const std::string & p_str, const char p_clapboard )
{
    return split( p_str, std::string( 1, p_clapboard ) );
}
