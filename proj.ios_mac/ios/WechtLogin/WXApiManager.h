//
//  WXApiManager.h
//  dadpat
//
//  Created by 吴思 on 2018/7/24.
//

#ifndef __WX_API_MANAGER_H__
#define __WX_API_MANAGER_H__
#import "WXApi.h"

#define AppID "wxc6d5d8bca1f1c5a9"
#define AppSecret "c5460ffde4c1c93dcc71392108bfa01d"

@interface WXApiManager : NSObject<WXApiDelegate>

+ (BOOL)sendAuthRequestScope:(NSString *)scope
                       State:(NSString *)state
                      OpenID:(NSString *)openID
            InViewController:(UIViewController *)viewController;

+ (instancetype)sharedManager;

@end

#endif /* __WX_API_MANAGER_H__ */
