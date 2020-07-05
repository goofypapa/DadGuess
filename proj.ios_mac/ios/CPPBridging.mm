//
//  CPPBridging.m
//  DadGuess-mobile
//
//  Created by 吴思 on 2019/4/2.
//

#import <Foundation/Foundation.h>

#import "Common.h"


void goSystemBlue()
{
    //    NSURL *url = [NSURL URLWithString:@"App-Prefs:root=Bluetooth"];
    //    if ([[UIApplication sharedApplication] canOpenURL:url]){
    //        [[UIApplication sharedApplication] openURL:url options:@{} completionHandler:nil];
    //    }
}

bool whetherSupportNFC( void )
{
    return false;
}
bool whetherOpenNFC( void )
{
    return false;
}
void openNFC( void )
{
    
}

void goChrome( const std::string & p_url )
{
    NSURL *URL = [NSURL URLWithString:[NSString stringWithUTF8String:p_url.c_str()]];
    [[UIApplication sharedApplication]openURL:URL];
}
