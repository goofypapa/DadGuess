// ---------------------------------------
// Sprite definitions for Main
// Generated with TexturePacker 4.8.3
//
// https://www.codeandweb.com/texturepacker
// 
// $TexturePacker:SmartUpdate:6946c8cba43af226d524c4bce4e203d4:d125f2b84a4b7e8c9df714fc75e50145:d8d629a6b501bd991840527e921c506f$
// ---------------------------------------

#ifndef __MAIN_H__
#define __MAIN_H__

#include "cocos2d.h"

namespace TexturePacker
{

class Main
{
public:
    /**
     * Add sprite frames contained in theMain sheet to
     * the SpriteFrameCache.
     */
    static void addSpriteFramesToCache();

    /**
     * Remove sprite frames contained in theMain sheet from
     * the SpriteFrameCache.
     */
    static void removeSpriteFramesFromCache();

    // ---------------------
    // sprite name constants
    // ---------------------
    static const std::string mainBaike;
    static const std::string mainDadpat;
    static const std::string mainGame;
    static const std::string mainMessage;
    static const std::string mainNewMessage;
    static const std::string mainNotice;
    static const std::string mainSetting;
    static const std::string mainUserHeadBackground;

    // --------------
    // Sprite objects
    // --------------
    static cocos2d::Sprite *createMainBaikeSprite();
    static cocos2d::Sprite *createMainDadpatSprite();
    static cocos2d::Sprite *createMainGameSprite();
    static cocos2d::Sprite *createMainMessageSprite();
    static cocos2d::Sprite *createMainNewMessageSprite();
    static cocos2d::Sprite *createMainNoticeSprite();
    static cocos2d::Sprite *createMainSettingSprite();
    static cocos2d::Sprite *createMainUserHeadBackgroundSprite();

}; // class

}; // namespace

#endif // __MAIN_H__
