// ---------------------------------------
// Sprite definitions for Main
// Generated with TexturePacker 4.8.2
//
// https://www.codeandweb.com/texturepacker
// 
// $TexturePacker:SmartUpdate:aacda3f6c1d81a39ff7ff166d2cbd371:d8cf3c763e467a84d584030d57634c1c:d8d629a6b501bd991840527e921c506f$
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

}; // class

}; // namespace

#endif // __MAIN_H__
