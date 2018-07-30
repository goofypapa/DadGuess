//
//  Common.hpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/7/6.
//
#ifndef __COMMON_H__
#define __COMMON_H__

#include "cocos2d.h"
#include <string>
#include "ui/CocosGUI.h"
#include <functional>
#include <map>


USING_NS_CC;

struct ButtonTouchEvents
{
public:
    std::function<void( cocos2d::Ref * )> onTouchBegin;
};

float adaptation( Vec2 p_sourcePixel = Vec2( 1920.0f, 1080.0f ) );

void touchAnswer( cocos2d::ui::Button * p_button, std::function<void( cocos2d::Ref * )> p_onTouchBegin );

//分割字符串
std::vector<std::string> split( const std::string & p_str, const std::string & p_clapboard );
std::vector<std::string> split( const std::string & p_str, const char p_clapboard );


#endif //__COMMON_H__
