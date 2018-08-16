/* DO NOT EDIT THIS FILE - it is machine generated */
#include "AppDelegate.h"
#include "cocos2d.h"

#if COCOS2D_VERSION >= 0x00030000
#include "base/CCEventType.h"
#else
#include "CCEventType.h"
#endif

#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <android/log.h>
#include "C2DXShareSDKTypeDef.h"
#include "CCJSONConverter.h"

USING_NS_CC;
using namespace cn::sharesdk;

/* Header for class ShareSDKUtils */
#ifndef _Included_ShareSDKUtils
#define _Included_ShareSDKUtils
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     aaa_bbb_Main
 * Method:    onJavaCallback
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_cn_sharesdk_ShareSDKUtils_onJavaCallback
  (JNIEnv * env, jclass thiz, jstring resp);

void callBackComplete(int reqID, int action, int platformId, C2DXDictionary* res);

void callBackError(int reqID, int action, int platformId, C2DXDictionary* res);

void callBackCancel(int reqID, int action, int platformId, C2DXDictionary* res);

bool getMethod(JniMethodInfo &mi, const char *methodName, const char *paramCode);

void releaseMethod(JniMethodInfo &mi);

//jni methods

bool initSDKJNI(const char* appKey,const char* appSecret);

bool setPlatformConfigJNI(int platformId, C2DXDictionary *platConfig);

bool registerAppAndSetPlatformConfigJNI(const char* appKey,const char* appSecret, C2DXDictionary *platformInfos);

bool authorizeJNI(int reqID, int platformId, C2DXAuthResultEvent callback);

bool cancelAuthorizeJNI(int platformId);

bool isAuthorizedValidJNI(int platformId);

bool isClientValidJNI(int platformId);

bool getUserInfoJNI(int reqID, int platformId, C2DXGetUserInfoResultEvent callback);

bool addFriendJNI(int reqID, int platformId, const char* account, C2DXAddFriendResultEvent callback);

C2DXDictionary* getAuthInfoJNI(int platformId);

bool shareContentJNI(int reqID, C2DXArray *platTypes, C2DXDictionary *content, C2DXShareResultEvent callback);

bool getFriendListJNI(int reqID, int platformId, int count, int page, C2DXGetFriendsResultEvent callback);

bool onekeyShareJNI(int reqID, int platformId, C2DXDictionary *content, C2DXShareResultEvent callback);

bool disableSSOWhenAuthorizeJNI(bool isDiaableSSO);

void toastShowJNI(const char* msg);


#ifdef __cplusplus
}
#endif
#endif