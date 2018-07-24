//
//  WXApiManager.h
//  dadpat
//
//  Created by 吴思 on 2018/7/24.
//

#ifndef __WX_API_MANAGER_H__
#define __WX_API_MANAGER_H__
#import "WXApi.h"

@interface WXApiManager : NSObject<WXApiDelegate>

+ (BOOL)sendAuthRequestScope:(NSString *)scope
                       State:(NSString *)state
                      OpenID:(NSString *)openID
            InViewController:(UIViewController *)viewController;

+ (instancetype)sharedManager;

@end

#endif /* __WX_API_MANAGER_H__ */
