//
//  LoginThridParty.m
//  dadpat-mobile
//
//  Created by 吴思 on 2018/7/23.
//
#import <Foundation/Foundation.h>
#import "WechtLogin/WXApiManager.h"


bool installedWecht( void )
{
    return [[UIApplication sharedApplication] canOpenURL:[NSURL URLWithString:@"weixin://"]] == YES;
}

bool loginWecht( void )
{
    return [WXApiManager sendAuthRequestScope: @"snsapi_message,snsapi_userinfo,snsapi_friend,snsapi_contact" State:@"xxx" OpenID:@"wxc6d5d8bca1f1c5a9" InViewController:nil];
}
