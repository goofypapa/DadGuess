/****************************************************************************
 Copyright (c) 2010-2013 cocos2d-x.org
 Copyright (c) 2013-2016 Chukong Technologies Inc.
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

#import "AppController.h"
#import "cocos2d.h"
#import "AppDelegate.h"
#import "RootViewController.h"
#import "RootViewControllerV.h"

#import "platform/ios/CCEAGLView-ios.h"
#import "Common.h"
#import <functional>

#import <UserNotifications/UserNotifications.h>
#import <CoreLocation/CoreLocation.h>

@implementation AppController

@synthesize window;

#pragma mark -
#pragma mark Application lifecycle

// cocos2d application instance
static AppDelegate s_sharedApplication;
static AppController *s_self;

static NetWorkStateListener::NetWorkState s_netWorkState = NetWorkStateListener::NetWorkState::NotReachable;
static std::function< void ( NetWorkStateListener::NetWorkState ) > s_networkStateListener = nullptr;

static PhoneStateListener::PhoneState s_phoneState = PhoneStateListener::PhoneState::IDLE;
static std::function< void ( PhoneStateListener::PhoneState ) > s_phoneStateListener = nullptr;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    
    s_self = self;
    //监听网络状态
    [self listenNetWorkingStatus];
    
    cocos2d::Application *app = cocos2d::Application::getInstance();
    
    // Initialize the GLView attributes
    app->initGLContextAttrs();
    cocos2d::GLViewImpl::convertAttrs();
    
    // Override point for customization after application launch.

    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];

    // Use RootViewController to manage CCEAGLView
    _viewController = [[RootViewController alloc]init];
    _viewController.wantsFullScreenLayout = YES;
    
    _viewControllerV = [[RootViewControllerV alloc]init];
    _viewControllerV.wantsFullScreenLayout = YES;
    

    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        [window addSubview: _viewController.view];
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:_viewController];
    }

    [window makeKeyAndVisible];

    [[UIApplication sharedApplication] setStatusBarHidden:true];
    [UIApplication sharedApplication].idleTimerDisabled=YES;
    
    // IMPORTANT: Setting the GLView should be done after creating the RootViewController
    cocos2d::GLView *glview = cocos2d::GLViewImpl::createWithEAGLView((__bridge void *)_viewController.view);
    
    //开启多点触控
    CCEAGLView *eaglview = (__bridge CCEAGLView *)glview->getEAGLView();
    [eaglview setMultipleTouchEnabled:YES];
    cocos2d::Director::getInstance()->setOpenGLView(glview);
    
    
    //后台运行player
    [self player];
    
    //run the cocos2d-x game scene
    app->run();

    return YES;
}

- (UIInterfaceOrientationMask)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(UIWindow *)window {
    return UIInterfaceOrientationMaskAllButUpsideDown;
}

-(void)listenNetWorkingStatus
{
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(reachabilityChanged:) name:kReachabilityChangedNotification object:nil];
    // 设置网络检测的站点
    NSString *remoteHostName = @"www.baidu.com";
    
    self.hostReachability = [Reachability reachabilityWithHostName:remoteHostName];
    [self.hostReachability startNotifier];
    
    self.internetReachability = [Reachability reachabilityForInternetConnection];
    [self.internetReachability startNotifier];
}

/*!
 * Called by Reachability whenever status changes.
 */
- (void) reachabilityChanged:(NSNotification *)note
{
    Reachability* curReach = [note object];
    [curReach isKindOfClass:[Reachability class]];
//    NSParameterAssert([curReach isKindOfClass:[Reachability class]]);
    [self updateInterfaceWithReachability:curReach];
}


- (void)updateInterfaceWithReachability:(Reachability *)reachability
{
    if (reachability == self.hostReachability || reachability == self.internetReachability )
    {
        NetworkStatus netStatus = [reachability currentReachabilityStatus];
        BOOL connectionRequired = [reachability connectionRequired];
        
        switch (netStatus)
        {
            case NotReachable:        {
                s_netWorkState = NetWorkStateListener::NetWorkState::NotReachable;
                break;
            }
                
            case ReachableViaWWAN:        {
                s_netWorkState = NetWorkStateListener::NetWorkState::WWAN;
                break;
            }
            case ReachableViaWiFi:        {
                s_netWorkState = NetWorkStateListener::NetWorkState::WiFi;
                break;
            }
        }
        
        if( connectionRequired == YES )
        {
            s_netWorkState = NetWorkStateListener::NetWorkState::Unknown;
        }
        
        
        if( s_networkStateListener )
        {
            s_networkStateListener( s_netWorkState );
        }
        
        printf( "net state: %d \n", s_netWorkState );
    }
}

- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
    // We don't need to call this method any more. It will interrupt user defined game pause&resume logic
    /* cocos2d::Director::getInstance()->pause(); */
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
    // We don't need to call this method any more. It will interrupt user defined game pause&resume logic
    /* cocos2d::Director::getInstance()->resume(); */
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    
    [self startBgTask];
    /** 播放声音 */
    [self.player play];
    
    cocos2d::Application::getInstance()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    cocos2d::Application::getInstance()->applicationWillEnterForeground();
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
    
    [UIApplication sharedApplication].idleTimerDisabled=NO;
}

//---------------------------

//------

+(void)changeRootViewControllerH
{
    CCEAGLView *__glView = (CCEAGLView *)s_self->_viewControllerV.view;
    s_self->_viewControllerV.view = nil;
    s_self->_viewController.view = __glView;
    
    if ([[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        [s_self->window addSubview:s_self->_viewController.view];
    }
    [s_self->window setRootViewController:s_self->_viewController];
    
    auto glview = Director::getInstance()->getOpenGLView();
    
//    cocos2d::Application::getInstance()->applicationScreenSizeChanged( frameSize.height, frameSize.width );
    
    auto frameSize = glview->getFrameSize();
    glview->setFrameSize( frameSize.height , frameSize.width );
    
    auto designResolutionSize = glview->getDesignResolutionSize();
    glview->setDesignResolutionSize( designResolutionSize.height, designResolutionSize.width, ResolutionPolicy::NO_BORDER );
    

}

+(void)changeRootViewControllerV
{
    CCEAGLView *__glView = (CCEAGLView *)s_self->_viewController.view;
    s_self->_viewController.view = nil;
    s_self->_viewControllerV.view = __glView;
    
    if ([[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        [s_self->window addSubview:s_self->_viewControllerV.view];
    }
    [s_self->window setRootViewController:s_self->_viewControllerV];
    
    auto glview = Director::getInstance()->getOpenGLView();
    
//    cocos2d::Application::getInstance()->applicationScreenSizeChanged( frameSize.height, frameSize.width );
    
    auto frameSize = glview->getFrameSize();
    glview->setFrameSize( frameSize.height , frameSize.width );

    auto designResolutionSize = glview->getDesignResolutionSize();
    glview->setDesignResolutionSize( designResolutionSize.height, designResolutionSize.width, ResolutionPolicy::NO_BORDER );
    
}

//----------------------------

- (AVAudioPlayer *)player{
    if (!_player){
        NSURL *url=[[NSBundle mainBundle]URLForResource:@"empty.wav" withExtension:nil];
        _player = [[AVAudioPlayer alloc]initWithContentsOfURL:url error:nil];
        [_player prepareToPlay];
        //一直循环播放
        _player.numberOfLoops = -1;
        AVAudioSession *session = [AVAudioSession sharedInstance];
        [session setCategory:AVAudioSessionCategoryPlayback error:nil];
        
        [session setActive:YES error:nil];
    }
    return _player;
}


- (void)startBgTask{
    UIApplication *application = [UIApplication sharedApplication];
    __block    UIBackgroundTaskIdentifier bgTask;
    bgTask = [application beginBackgroundTaskWithExpirationHandler:^{
        //这里延迟的系统时间结束
        [application endBackgroundTask:bgTask];
        NSLog(@"%f",application.backgroundTimeRemaining);
    }];
    
}
//----------------------------


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}


#if __has_feature(objc_arc)
#else
- (void)dealloc {
    [window release];
    [_viewController release];
    [_viewControllerV release];
    [super dealloc];
}
#endif


@end

void setAppOrientation( const bool p_isPortrait )
{
    if( p_isPortrait )
    {
        [AppController changeRootViewControllerV];
    }else{
        [AppController changeRootViewControllerH];
    }
}


std::string createUUID( void )
{
    CFUUIDRef t_puuid = CFUUIDCreate( nil );
    CFStringRef t_uuidString = CFUUIDCreateString( nil, t_puuid );
    NSString * t_NSString = (NSString *)CFBridgingRelease( CFStringCreateCopy( NULL, t_uuidString) );

    const char  *  t_cString = [t_NSString UTF8String];
    return std::string( t_cString );
}

NetWorkStateListener::NetWorkState getNetWorkState( void )
{
    return s_netWorkState;
}

void bindNetWorkStateListener( std::function< void( NetWorkStateListener::NetWorkState ) > p_networkStateListener )
{
    s_networkStateListener = p_networkStateListener;
}

void unbindNetWorkStateListener( void )
{
    s_networkStateListener = nullptr;
}

PhoneStateListener::PhoneState getPhoneState( void )
{
    return s_phoneState;
}

void bindPhoneStateListener( std::function< void( PhoneStateListener::PhoneState ) > p_phoneStateListener )
{
    s_phoneStateListener = p_phoneStateListener;
}

void unbindPhoneStateListener( void )
{
    s_phoneStateListener = nullptr;
}
