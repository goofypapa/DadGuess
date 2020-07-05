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

#import <UIKit/UIKit.h>
#import "Reachability.h"
#import <AVFoundation/AVFoundation.h>

@class RootViewController;
@class RootViewControllerV;

@interface AppController : NSObject <UIApplicationDelegate> {

}

@property(nonatomic, readonly) RootViewController* viewController;
@property(nonatomic, readonly) RootViewControllerV* viewControllerV;//声明实例变量

@property (assign, nonatomic) Reachability *hostReachability;
@property (assign, nonatomic) Reachability *internetReachability;

@property (nonatomic) dispatch_source_t badgeTimer;
@property (nonatomic, strong) AVAudioPlayer *player;

+(void)changeRootViewControllerH;//静态方法(修改屏幕为横屏)
+(void)changeRootViewControllerV;//静态方法(修改屏幕为竖屏)

@end

