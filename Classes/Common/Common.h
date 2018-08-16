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
#include "C2DXShareSDK.h"

#ifdef __ANDROID_NDK__
#include "android/log.h"

#define  LOG_TAG    "main"
#define  printf(...)  __android_log_print( ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__ )

#endif


USING_NS_CC;

//工具

struct ButtonTouchEvents
{
public:
    std::function<void( cocos2d::Ref * )> onTouchBegin;
    std::function<void( cocos2d::Ref * )> onTouchCanceled;
    std::function<void( cocos2d::Ref * )> onTouchEnded;
    std::function<void( cocos2d::Ref * )> onTouchMoved;
};

float adaptation( Vec2 p_sourcePixel = Vec2( 1920.0f, 1080.0f ) );

void touchAnswer( cocos2d::ui::Button * p_button, std::function<void( cocos2d::Ref * )> p_onTouchEnd, float p_scaleTo = 1.2f, float p_sacleOriginal = 1.0f );
void touchAnswer( cocos2d::ui::Button * p_button, std::function<void( cocos2d::Ref * )> p_onTouchBegin, std::function<void( cocos2d::Ref * )> p_onTouchCanceled, std::function<void( cocos2d::Ref * )> p_onTouchEnded, std::function<void( cocos2d::Ref * )> p_onTouchMoved );

//分割字符串
std::vector<std::string> split( const std::string & p_str, const std::string & p_clapboard );
std::vector<std::string> split( const std::string & p_str, const char p_clapboard );

void setAppOrientation( const bool p_isPortrait );


//shareSDK 工具
std::string toString( __Dictionary & p_dirtionary );
std::string toString( Ref * p_obj );



#endif //__COMMON_H__
