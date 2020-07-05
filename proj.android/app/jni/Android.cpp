//
// Created by 吴思 on 2018/8/14.
//

#include "Common.h"
#include "BlueDeviceListener.h"
#include "NFCDeviceListener.h"
#include "platform/android/jni/JniHelper.h"
#include "DadGuessMainScene.h"
#include <string.h>
#include <map>
#include <thread>
#include <mutex>
#include "PlayAudio.h"
#include "AudioEngine.h"

using namespace cocos2d;
using namespace cocos2d::experimental;

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

void BlueDeviceListener::deconnect( void )
{
    JniMethodInfo info;
    bool ret = JniHelper::getStaticMethodInfo(info,"org/cocos2dx/lib/BluePackage","deconnect","()V");
    if(ret)
    {
        info.env->CallStaticVoidMethod(info.classID,info.methodID);
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


//
static PhoneStateListener::PhoneState s_phoneState = PhoneStateListener::PhoneState::IDLE;
static std::function< void( PhoneStateListener::PhoneState ) > s_phoneListener = nullptr;
void bindPhoneStateListener( std::function< void( PhoneStateListener::PhoneState ) > p_phoneStateListener  )
{
    s_phoneListener = p_phoneStateListener;
}
void unbindPhoneStateListener( void )
{
    s_phoneListener = nullptr;
}

PhoneStateListener::PhoneState getPhoneState( void )
{
    return s_phoneState;
}

static std::map< std::string, std::pair< HttpCallBack, HttpCallBack > > s_httpRequestPool;
static std::mutex s_httpRequestMutex;
void httpGet( const std::string & p_url, const std::string & p_token, const std::string & p_requestId, HttpCallBack p_callBackSuccess, HttpCallBack p_callBackFinal )
{
    JniMethodInfo info;
    bool ret = JniHelper::getStaticMethodInfo(info,"org/cocos2dx/cpp/Android","httpGet","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    if(ret)
    {
        jstring t_url = info.env->NewStringUTF( p_url.c_str() );
        jstring t_token = info.env->NewStringUTF( p_token.c_str() );
        jstring t_requestId = info.env->NewStringUTF( p_requestId.c_str() );
        info.env->CallStaticVoidMethod(info.classID,info.methodID, t_url, t_token, t_requestId);
    }

    auto t_tmp = std::pair< HttpCallBack, HttpCallBack >( p_callBackSuccess, p_callBackFinal );
    s_httpRequestMutex.lock();
    s_httpRequestPool[ p_requestId ] = t_tmp;
    s_httpRequestMutex.unlock();
}

void httpPost( const std::string & p_url, const std::string & p_data, const std::string & p_token, const std::string & p_requestId, HttpCallBack p_callBackSuccess, HttpCallBack p_callBackFinal )
{
    JniMethodInfo info;
    bool ret = JniHelper::getStaticMethodInfo(info,"org/cocos2dx/cpp/Android","httpPost","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    if(ret)
    {
        jstring t_url = info.env->NewStringUTF( p_url.c_str() );
        jstring t_data = info.env->NewStringUTF( p_data.c_str() );
        jstring t_token = info.env->NewStringUTF( p_token.c_str() );
        jstring t_requestId = info.env->NewStringUTF( p_requestId.c_str() );
        info.env->CallStaticVoidMethod(info.classID,info.methodID, t_url, t_data, t_token, t_requestId);
    }
    auto t_tmp = std::pair< HttpCallBack, HttpCallBack >( p_callBackSuccess, p_callBackFinal );
    s_httpRequestMutex.lock();
    s_httpRequestPool[ p_requestId ] = t_tmp;
    s_httpRequestMutex.unlock();
}

struct DownloadCallBackFuns{
    HttpDownloadStartCallBack callBackStart;
    HttpDownloadEndCallBack callBackEnd;
    HttpDownloadFinalCallBack callBackFinal;
    HttpDownloadRateCallBack callBackRate;
};
static std::map< std::string, DownloadCallBackFuns > s_httpDownloadPool;
static std::mutex s_httpDownloadMutex;
void httpDownload(  const std::string & p_taskId, const std::string & p_url, const std::string & p_filePath, HttpDownloadStartCallBack p_callBackStart, HttpDownloadEndCallBack p_callBackEnd, HttpDownloadFinalCallBack p_callBackFinal, HttpDownloadRateCallBack p_callBackRate )
{
    JniMethodInfo info;
    bool ret = JniHelper::getStaticMethodInfo(info,"org/cocos2dx/cpp/Android","httpDownload","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    if(ret)
    {
        jstring t_taskId = info.env->NewStringUTF( p_taskId.c_str() );
        jstring t_url = info.env->NewStringUTF( p_url.c_str() );
        jstring t_filePath = info.env->NewStringUTF( p_filePath.c_str() );

        info.env->CallStaticVoidMethod(info.classID,info.methodID, t_taskId, t_url, t_filePath);
    }

    DownloadCallBackFuns t_downloadCallBackFuns{ p_callBackStart, p_callBackEnd, p_callBackFinal, p_callBackRate };
    s_httpDownloadMutex.lock();
    s_httpDownloadPool[ p_taskId ] = t_downloadCallBackFuns;
    s_httpDownloadMutex.unlock();
}

bool whetherSupportNFC( void )
{
    bool t_result = false;
    JniMethodInfo info;
    bool ret = JniHelper::getStaticMethodInfo(info,"org/cocos2dx/cpp/Android","whetherSupportNFC","()Z");
    if(ret)
    {
        t_result = info.env->CallStaticBooleanMethod(info.classID,info.methodID);
    }

    return t_result;
}

bool whetherOpenNFC( void )
{
    bool t_result = false;
    JniMethodInfo info;
    bool ret = JniHelper::getStaticMethodInfo(info,"org/cocos2dx/cpp/Android","whetherOpenNFC","()Z");
    if(ret)
    {
        t_result = info.env->CallStaticBooleanMethod(info.classID,info.methodID);
    }

    return t_result;
}

void openNFC( void )
{
    JniMethodInfo info;
    bool ret = JniHelper::getStaticMethodInfo(info,"org/cocos2dx/cpp/Android","openNFC","()V");
    if(ret)
    {
        info.env->CallStaticVoidMethod(info.classID,info.methodID);
    }
}

void goSystemBlue( void )
{

}

void goChrome( const std::string & p_url )
{
    JniMethodInfo info;
    bool ret = JniHelper::getStaticMethodInfo(info,"org/cocos2dx/cpp/Android","goChrome","(Ljava/lang/String;)V");
    if(ret)
    {
        jstring t_url = info.env->NewStringUTF( p_url.c_str() );
        info.env->CallStaticVoidMethod( info.classID, info.methodID, t_url );
    }
}

int PlayAudio::play( const std::string & p_audioPath )
{
    return AudioEngine::play2d( p_audioPath );
}

void PlayAudio::pause( const int p_playId )
{
    AudioEngine::pause( p_playId );
}

void PlayAudio::pauseAll()
{
    AudioEngine::pauseAll();
}

void PlayAudio::resume( const int p_playId )
{
    AudioEngine::resume( p_playId );
}

void PlayAudio::resumeAll()
{
    AudioEngine::resumeAll();
}

void PlayAudio::stop( const int p_playId )
{
    AudioEngine::stop( p_playId );
}

void PlayAudio::stopAll( const int p_playId )
{
    AudioEngine::stopAll();
}

PlayAudio::AudioState PlayAudio::getState( const int p_playId )
{
    AudioEngine::AudioState t_state = AudioEngine::getState( p_playId );
    switch( t_state )
    {
        case AudioEngine::AudioState::ERROR:
            return PlayAudio::AudioState::ERROR;
        case AudioEngine::AudioState::INITIALIZING:
            return PlayAudio::AudioState::INITIALIZING;
        case AudioEngine::AudioState::PLAYING:
            return PlayAudio::AudioState::PLAYING;
        case AudioEngine::AudioState::PAUSED:
            return PlayAudio::AudioState::PAUSED;
    }

    return PlayAudio::AudioState::ERROR;
}

void PlayAudio::setFinishCallback( int p_playId, const FINISH_CALLBACK_FUNC & p_finishCallbackFunc )
{
    AudioEngine::setFinishCallback( p_playId, p_finishCallbackFunc );
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

    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_PhoneBroadcastReceiver_phoneStateChange(JNIEnv *env, jobject clazz, jint phoneState)
    {
        PhoneStateListener::PhoneState t_phoneState = PhoneStateListener::PhoneState::IDLE;

        switch( phoneState )
        {
            case 0:
                t_phoneState = PhoneStateListener::PhoneState::IDLE;
            break;
            case 1:
                t_phoneState = PhoneStateListener::PhoneState::RINGING;
            break;
            case 2:
                t_phoneState = PhoneStateListener::PhoneState::OFFHOOK;
            break;
            default:
                t_phoneState = PhoneStateListener::PhoneState::IDLE;
            break;
        }

        if( s_phoneState == t_phoneState )
        {
            return;
        }

        s_phoneState = t_phoneState;

        if( s_phoneListener )
        {
            s_phoneListener( s_phoneState );
        }
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_BluePackage_scanedDevice( JNIEnv *env, jobject clazz, jstring p_deviceAddess, jstring p_deviceName )
    {
        std::string t_deviceAddess = jstringToChar( env, p_deviceAddess );
        std::string t_deviceName = jstringToChar( env, p_deviceName );

        Director::getInstance()->getScheduler()->performFunctionInCocosThread([t_deviceAddess, t_deviceName]{
            BlueDeviceListener::_onScanDevice( t_deviceAddess, t_deviceName );
        });

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
        bool t_state = !p_connectState;
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([t_state]{
            BlueDeviceListener::_onConnectStateChanged( t_state );
        });
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_NfcUtils_NFCStateChange( JNIEnv *env, jobject clazz, jboolean p_state )
    {
        static bool s_NFCState = false;
        if( s_NFCState == p_state )
        {
            return;
        }

        s_NFCState = p_state;

        NFCDeviceListener::NfcStateChange( s_NFCState );
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Http_HttpResponse(JNIEnv *env, jobject clazz, jboolean p_state, jstring p_requestId, jstring p_resHandler, jstring p_resBody)
    {

        std::string t_requestId = jstringToChar( env, p_requestId );
        std::string t_resHandler = jstringToChar( env, p_resHandler );
        std::string t_resBody = jstringToChar( env, p_resBody );
        auto t_it = s_httpRequestPool.find( t_requestId );
        if( t_it == s_httpRequestPool.end() )
        {
            return;
        }

        printf( "---------------> p_state: %d", p_state );

        auto t_callBack = t_it->second.second;
        if( p_state != 0 )
        {
            t_callBack = t_it->second.first;
        }else{
            return;
        }

        std::thread( [=](){
            t_callBack( t_requestId, t_resHandler, t_resBody );
        } ).detach();

        s_httpRequestMutex.lock();
        s_httpRequestPool.erase( t_it );
        s_httpRequestMutex.unlock();
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Http_HttpDownloadStart(JNIEnv *env, jobject clazz, jstring p_taskId, jint p_fileSize)
    {
        std::string t_taskId = jstringToChar( env, p_taskId );
        auto t_fileSize = p_fileSize;
        auto t_it = s_httpDownloadPool.find( t_taskId );
        if( t_it == s_httpDownloadPool.end() )
        {
            return;
        }

        auto t_callBackStart = t_it->second.callBackStart;
        if( t_callBackStart )
        {
            std::thread( [=](){
                        t_callBackStart( t_taskId, t_fileSize );
                    } ).detach();
        }
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Http_HttpDownloadEnd(JNIEnv *env, jobject clazz, jstring p_taskId)
    {
        std::string t_taskId = jstringToChar( env, p_taskId );
        auto t_it = s_httpDownloadPool.find( t_taskId );
        if( t_it == s_httpDownloadPool.end() )
        {
            return;
        }

        auto t_callBackEnd = t_it->second.callBackEnd;
        if( t_callBackEnd )
        {
            std::thread( [=](){
                        t_callBackEnd( t_taskId );
                    } ).detach();
        }

        s_httpDownloadMutex.lock();
        s_httpDownloadPool.erase( t_taskId );
        s_httpDownloadMutex.unlock();
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Http_HttpDownloadFinal(JNIEnv *env, jobject clazz, jstring p_taskId, jstring p_msg)
    {
        std::string t_taskId = jstringToChar( env, p_taskId );
        std::string t_msg = jstringToChar( env, p_msg );
        auto t_it = s_httpDownloadPool.find( t_taskId );
        if( t_it == s_httpDownloadPool.end() )
        {
            return;
        }

        auto t_callBackFinal = t_it->second.callBackFinal;
        if( t_callBackFinal )
        {
            std::thread( [=](){
                        t_callBackFinal( t_taskId, t_msg );
                    } ).detach();
        }
        s_httpDownloadMutex.lock();
        s_httpDownloadPool.erase( t_taskId );
        s_httpDownloadMutex.unlock();
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Http_HttpDownloadRate(JNIEnv *env, jobject clazz, jstring p_taskId, jfloat p_rate)
    {
        std::string t_taskId = jstringToChar( env, p_taskId );
        float t_rate = p_rate;
        auto t_it = s_httpDownloadPool.find( t_taskId );
        if( t_it == s_httpDownloadPool.end() )
        {
            return;
        }

        auto t_callBackRate = t_it->second.callBackRate;
        if( t_callBackRate )
        {
            std::thread( [=](){
                        t_callBackRate( t_taskId, t_rate );
                    } ).detach();
        }
    }

    JNIEXPORT void JNICALL JNICALL Java_org_cocos2dx_lib_Cocos2dxActivity_scanCard( JNIEnv *env, jobject clazz, jint p_cardId )
    {
        NFCDeviceListener::NfcScanCard( p_cardId );
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
    std::string t_result = "";
     std::thread( [&t_result](){
        JniMethodInfo info;
        //getStaticMethodInfo判断java定义的静态函数是否存在，返回bool
        bool ret = JniHelper::getStaticMethodInfo(info,"org/cocos2dx/cpp/Android","createUUID","()Ljava/lang/String;");
        if(ret)
        {
            //传入类ID和方法ID，小心方法名写错，第一个字母是大写
            jstring t_str = (jstring)info.env->CallStaticObjectMethod(info.classID,info.methodID);

            char * t_c = jstringToChar( info.env, t_str);
            t_result = t_c;
        }
     }).join();

    return t_result;
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