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

#define PI 3.1415926535897932385f

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
void clearTouchAnswer( void );

//分割字符串
std::vector<std::string> split( const std::string & p_str, const std::string & p_clapboard );
std::vector<std::string> split( const std::string & p_str, const char p_clapboard );

//sql 字符串转义
std::string strToSqlStr( const std::string & p_str );
std::string sqlStrToStr( const std::string & p_sqlStr );

void setAppOrientation( const bool p_isPortrait );

std::string createUUID( void );

typedef std::function<void( const std::string & p_requestId, const std::string & p_res )> HttpCallBack;
void httpGet( const std::string & p_url, const std::string & p_token, const std::string & p_requestId, HttpCallBack p_callBackSuccess, HttpCallBack p_callBackFinal );
void httpPost( const std::string & p_url, const std::string & p_data, const std::string & p_token, const std::string & p_requestId, HttpCallBack p_callBackSuccess, HttpCallBack p_callBackFinal );

typedef std::function<void( const std::string & p_taskId, const int p_fileSize )> HttpDownloadStartCallBack;
typedef std::function<void( const std::string & p_taskId )> HttpDownloadEndCallBack;
typedef std::function<void( const std::string & p_taskId, const std::string & p_meg )> HttpDownloadFinalCallBack;
typedef std::function<void( const std::string & p_taskId, const float p_downloadRate )> HttpDownloadRateCallBack;

void httpDownload(  const std::string & p_taskId, const std::string & p_url, const std::string & p_filePath, HttpDownloadStartCallBack p_callBackStart, HttpDownloadEndCallBack p_callBackEnd, HttpDownloadFinalCallBack p_callBackFinal, HttpDownloadRateCallBack p_callBackRate );

bool whetherSupportNFC( void );
bool whetherOpenNFC( void );
void openNFC( void );

//shareSDK 工具
std::string toString( __Dictionary & p_dirtionary );
std::string toString( Ref * p_obj );

const std::string & writeFilePath( void );
std::string fullFilePath( const std::string & p_file );

std::string fileSuffix( const std::string p_fileName );

namespace NetWorkStateListener
{
    enum NetWorkState{ WWAN = 0, WiFi, NotReachable, Unknown };
}
NetWorkStateListener::NetWorkState getNetWorkState( void );

void bindNetWorkStateListener( std::function< void( NetWorkStateListener::NetWorkState ) > );
void unbindNetWorkStateListener( void );

cocos2d::Color4F createColor4FWithStr( const std::string & p_colorStr, const float p_alpha = 1.0f );

//跳转到系统蓝牙设置页面
void goSystemBlue( void );


#endif //__COMMON_H__
