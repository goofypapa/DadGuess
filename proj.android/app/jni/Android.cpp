//
// Created by 吴思 on 2018/8/14.
//

#include "Common.h"
#include "platform/android/jni/JniHelper.h"

char* jstringToChar(JNIEnv* env, jstring jstr);

void setAppOrientation( const bool p_isPortrait )
{

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
    jstring strencode = env->NewStringUTF("GB2312");
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