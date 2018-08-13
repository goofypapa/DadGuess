//
//  WXApiManager.m
//  dadpat-mobile
//
//  Created by 吴思 on 2018/7/24.
//

#import "WXApiManager.h"
#import "LoginScene.h"

@implementation WXApiManager

#pragma mark - LifeCycle
+(instancetype)sharedManager {
    static dispatch_once_t onceToken;
    static WXApiManager *instance;
    dispatch_once(&onceToken, ^{
        instance = [[WXApiManager alloc] init];
    });
    return instance;
}

+ (BOOL)sendAuthRequestScope:(NSString *)scope
                       State:(NSString *)state
                      OpenID:(NSString *)openID
            InViewController:(UIViewController *)viewController {
    SendAuthReq* req = [[SendAuthReq alloc] init];
    req.scope = scope; // @"post_timeline,sns"
    req.state = state;
    req.openID = openID;
    
    return [WXApi sendAuthReq:req
               viewController:viewController
                     delegate:[WXApiManager sharedManager]];
}

- (void)onResp:(BaseResp *)resp
{
    auto authResp = static_cast<SendAuthResp*>(resp);
    if( authResp == nullptr )
    {
        printf( "read code final" );
        return;
    }
    const char * code = [[authResp code] UTF8String];
    
    printf( "onResp\n" );
    printf( "code: %s \n", code );
    LoginScene::loginWechatCallBack( code );
}

- (void)onReq:(BaseReq *)req
{
    printf( "onReq\n" );
}

@end
