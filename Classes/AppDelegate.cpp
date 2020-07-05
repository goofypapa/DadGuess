/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "AppDelegate.h"
#include "Login/LoginScene.h"
#include "DadGuess/DadGuessUpdateScene.h"
#include "C2DXShareSDK.h"
#include "SMSSDK.h"
#include "Config.h"
#include "DataTableUser.h"
#include "Common.h"
#include "BaseScene.h"

// #define USE_AUDIO_ENGINE 1
// #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;
using namespace smssdk;

static cocos2d::Size designResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    
    //shareSDK
    __Dictionary *totalDict = __Dictionary::create();

#ifdef __ANDROID_NDK__
    //微信
    __Dictionary *wechatConf = __Dictionary::create();
    wechatConf->setObject(__String::create("wxd6d78ef8accb5cf4"), "AppId");
    wechatConf->setObject(__String::create("938ae63610eea92a61f0496bec58d708"), "AppSecret");
    std::stringstream wechat;
    wechat << cn::sharesdk::C2DXPlatTypeWeChat;
    totalDict->setObject(wechatConf, wechat.str());

    //新浪微博
   __Dictionary *sinaWeiboConf= __Dictionary::create();
    sinaWeiboConf->setObject(__String::create("197904272"), "AppKey");
    sinaWeiboConf->setObject(__String::create("1645c87956e28423b05a595f2e100d26"), "AppSecret");
    sinaWeiboConf->setObject(__String::create("http://www.sharesdk.cn"), "RedirectUrl");
    sinaWeiboConf->setObject(__String::create("true"), "ShareByAppClient");
    sinaWeiboConf->setObject(__String::create("true"), "Enable");
    stringstream sina;
    sina << cn::sharesdk::C2DXPlatTypeSinaWeibo;
    totalDict->setObject(sinaWeiboConf, sina.str());
#else
    //微信
    __Dictionary *wechatConf = __Dictionary::create();
    wechatConf->setObject(__String::create("wxd6d78ef8accb5cf4"), "app_id");
    wechatConf->setObject(__String::create("938ae63610eea92a61f0496bec58d708"), "app_secret");
    std::stringstream wechat;
    wechat << cn::sharesdk::C2DXPlatTypeWechatPlatform;
    totalDict->setObject(wechatConf, wechat.str());
    
    //新浪微博
   __Dictionary *sinaWeiboConf= __Dictionary::create();
   sinaWeiboConf->setObject(__String::create("197904272"), "app_key");
   sinaWeiboConf->setObject(__String::create("1645c87956e28423b05a595f2e100d26"), "app_secret");
   sinaWeiboConf->setObject(__String::create("http://www.sharesdk.cn"), "redirect_uri");
   std::stringstream sina;
   sina << cn::sharesdk::C2DXPlatTypeSinaWeibo;
   totalDict->setObject(sinaWeiboConf, sina.str());
#endif

    const char * t_appKey = "2970ee5899ef8";
    const char * t_appSecret = "4eb673fc8993280113d9c4376535f83a";
    
    cn::sharesdk::C2DXShareSDK::registerAppAndSetPlatformConfig( t_appKey, t_appSecret, totalDict );
    SMSSDK::init( t_appKey, t_appSecret, false );
    
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("DadGuess", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("DadGuess");
#endif
        director->setOpenGLView(glview);
    }

//    // turn on display FPS
//    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    auto frameSize = glview->getFrameSize();
    // if the frame's height is larger than the height of medium size.
    if (frameSize.height > mediumResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > smallResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium size.
    else
    {        
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }

    FileUtils::getInstance()->addSearchPath( "shader" );
    FileUtils::getInstance()->addSearchPath( writeFilePath() );

    register_all_packages();

    // create a scene. it's an autorelease object
    Scene * t_startScene = nullptr;
    
    bool t_isLogin = LoginScene::isLogined();
    
    if( t_isLogin )
    {
        t_startScene = DadGuessUpdateScene::create();
    }else{
        t_startScene = LoginScene::CreateScene();
    }

    // run
    director->runWithScene( t_startScene );

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

// #if USE_AUDIO_ENGINE
//     AudioEngine::pauseAll();
// #elif USE_SIMPLE_AUDIO_ENGINE
//     SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
//     SimpleAudioEngine::getInstance()->pauseAllEffects();
// #endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

// #if USE_AUDIO_ENGINE
//     AudioEngine::resumeAll();
// #elif USE_SIMPLE_AUDIO_ENGINE
//     SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
//     SimpleAudioEngine::getInstance()->resumeAllEffects();
// #endif
}
