// ---------------------------------------
// Sprite definitions for Login
// Generated with TexturePacker 4.8.2
//
// https://www.codeandweb.com/texturepacker
// 
// $TexturePacker:SmartUpdate:279c47be3c46bf9c45f7df588e8d6a39:b5b429a8173aa0044a990c301d374b1c:1536d167d15a0cd66b16c85ed832fedf$
// ---------------------------------------

#ifndef __LOGIN_H__
#define __LOGIN_H__

#include "cocos2d.h"

namespace TexturePacker
{

class Login
{
public:
    /**
     * Add sprite frames contained in theLogin sheet to
     * the SpriteFrameCache.
     */
    static void addSpriteFramesToCache();

    /**
     * Remove sprite frames contained in theLogin sheet from
     * the SpriteFrameCache.
     */
    static void removeSpriteFramesFromCache();

    // ---------------------
    // sprite name constants
    // ---------------------
    static const std::string loginBorder;
    static const std::string loginButton;
    static const std::string loginEyeClose;
    static const std::string loginEyeOpen;
    static const std::string loginPhone;
    static const std::string loginSina;
    static const std::string loginTitle;
    static const std::string loginWechat;

    // --------------
    // Sprite objects
    // --------------
    static cocos2d::Sprite *createLoginBorderSprite();
    static cocos2d::Sprite *createLoginButtonSprite();
    static cocos2d::Sprite *createLoginEyeCloseSprite();
    static cocos2d::Sprite *createLoginEyeOpenSprite();
    static cocos2d::Sprite *createLoginPhoneSprite();
    static cocos2d::Sprite *createLoginSinaSprite();
    static cocos2d::Sprite *createLoginTitleSprite();
    static cocos2d::Sprite *createLoginWechatSprite();

}; // class

}; // namespace

#endif // __LOGIN_H__
