//
// Created by 吴思 on 2018/8/14.
//

#include "Common.h"
#include "BlueDeviceListener.h"
#include "platform/android/jni/JniHelper.h"
#include <string.h>

#include <functional>

char* jstringToChar(JNIEnv* env, jstring jstr);

unsigned char charToHex( char p_c );

void BlueDeviceListener::listen( void )
{
    JniMethodInfo info;
    bool ret = JniHelper::getStaticMethodInfo(info,"org/cocos2dx/lib/BluePackage","listen","()V");
    if(ret)
    {
        info.env->CallStaticVoidMethod(info.classID,info.methodID);
    }
}

void BlueDeviceListener::scan( void )
{
    JniMethodInfo info;
    bool ret = JniHelper::getStaticMethodInfo(info,"org/cocos2dx/lib/BluePackage","openDevice","()V");
    if(ret)
    {
        info.env->CallStaticVoidMethod(info.classID,info.methodID);
    }
}

void BlueDeviceListener::cancelScan( void )
{
    JniMethodInfo info;
    bool ret = JniHelper::getStaticMethodInfo(info,"org/cocos2dx/lib/BluePackage","stopScan","()V");
    if(ret)
    {
        info.env->CallStaticVoidMethod(info.classID,info.methodID);
    }
}

void BlueDeviceListener::connect( const std::string & p_deviceId )
{
    JniMethodInfo info;
    bool ret = JniHelper::getStaticMethodInfo(info,"org/cocos2dx/lib/BluePackage","connect","(Ljava/lang/String;)V");
    if(ret)
    {
        jstring param = info.env->NewStringUTF( p_deviceId.c_str() );
        info.env->CallStaticVoidMethod(info.classID,info.methodID, param);
    }
}


static NetWorkStateListener::NetWorkState s_netWorkState = NetWorkStateListener::NetWorkState::Unknown;
static std::function< void( NetWorkStateListener::NetWorkState ) > s_networkStateListener = nullptr;

void bindNetWorkStateListener( std::function< void( NetWorkStateListener::NetWorkState ) > p_networkStateListener )
{
    s_networkStateListener = p_networkStateListener;
}
void unbindNetWorkStateListener( void )
{
    s_networkStateListener = nullptr;
}

NetWorkStateListener::NetWorkState getNetWorkState( void )
{
    return s_netWorkState;
}

extern "C"
{
    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_NetBroadcastReceiver_netStateChange(JNIEnv *env, jobject clazz, jint netState)
    {
        NetWorkStateListener::NetWorkState t_netWorkState = NetWorkStateListener::NetWorkState::NotReachable;

        switch( netState )
        {
            case 0:
                t_netWorkState = NetWorkStateListener::NetWorkState::NotReachable;
            break;
            case 1:
                t_netWorkState = NetWorkStateListener::NetWorkState::WWAN;
            break;
            case 2:
                t_netWorkState = NetWorkStateListener::NetWorkState::WiFi;
            break;
            default:
                t_netWorkState = NetWorkStateListener::NetWorkState::Unknown;
            break;
        }

        if( s_netWorkState == t_netWorkState )
        {
            return;
        }

        s_netWorkState = t_netWorkState;

        if( s_networkStateListener )
        {
            s_networkStateListener( s_netWorkState );
        }
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_BluePackage_scanedDevice( JNIEnv *env, jobject clazz, jstring p_deviceAddess, jstring p_deviceName )
    {
        std::string t_deviceAddess = jstringToChar( env, p_deviceAddess );
        std::string t_deviceName = jstringToChar( env, p_deviceName );

        BlueDeviceListener::_onScanDevice( t_deviceAddess, t_deviceName );
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_BluePackage_recvData( JNIEnv *env, jobject clazz, jstring p_data )
    {
        char * t_source = jstringToChar( env, p_data );

        printf( "-------------> %s \n", t_source );

        std::vector< unsigned char > t_data;

        for( int i = 0; i < strlen( t_source ) - 1; i += 2 )
        {
            unsigned char t_char = ( charToHex( t_source[i] ) << 4 ) + charToHex( t_source[i + 1] ) ;
            t_data.push_back( t_char );
        }

        BlueDeviceListener::_onRecvedData( t_data );
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_BluePackage_connectDeviceStateChange( JNIEnv *env, jobject clazz, jint p_connectState )
    {
        BlueDeviceListener::_onConnectStateChanged( !p_connectState );
    }
}

void setAppOrientation( const bool p_isPortrait )
{
    JniMethodInfo info;
    //getStaticMethodInfo判断java定义的静态函数是否存在，返回bool
    
    bool ret = false;
    if( p_isPortrait )
    {
        ret = JniHelper::getStaticMethodInfo(info,"org/cocos2dx/lib/Cocos2dxActivity","ChangePort","()V");
    }else{
        ret = JniHelper::getStaticMethodInfo(info,"org/cocos2dx/lib/Cocos2dxActivity","ChangeLand","()V");
    }
    
    if(ret)
    {
        //传入类ID和方法ID，小心方法名写错，第一个字母是大写
        info.env->CallStaticVoidMethod(info.classID,info.methodID);

        auto director = Director::getInstance();
        auto glview = director->getOpenGLView();
        Size frameSize=glview->getFrameSize();
        glview->setFrameSize(frameSize.height,frameSize.width);

        auto designResolutionSize = glview->getDesignResolutionSize();
        glview->setDesignResolutionSize( designResolutionSize.height, designResolutionSize.width, ResolutionPolicy::NO_BORDER );

    }
}

std::string createUUID( void )
{

    JniMethodInfo info;
    //getStaticMethodInfo判断java定义的静态函数是否存在，返回bool
    bool ret = JniHelper::getStaticMethodInfo(info,"org/cocos2dx/cpp/Android","createUUID","()Ljava/lang/String;");
    if(ret)
    {
        //传入类ID和方法ID，小心方法名写错，第一个字母是大写
        jstring t_str = (jstring)info.env->CallStaticObjectMethod(info.classID,info.methodID);

        char * t_c = jstringToChar( info.env, t_str);
        return t_c;
    }

    return "";
}


char* jstringToChar(JNIEnv* env, jstring jstr)
{
    char* rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("UTF-8");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0)
    {
        rtn = (char*) malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    return rtn;
}

unsigned char charToHex( char p_c )
{
    if( p_c >= '0' && p_c <= '9' )
    {
        return p_c - '0';
    }
    if( p_c >= 'a' && p_c <= 'f' )
    {
        return 10 + p_c - 'a';
    }
    if( p_c >= 'A' && p_c <= 'F' )
    {
        return 10 + p_c - 'A';
    }
    return 0;
}