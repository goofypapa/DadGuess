// ---------------------------------------
// Sprite definitions for Personal
// Generated with TexturePacker 4.8.2
//
// https://www.codeandweb.com/texturepacker
// 
// $TexturePacker:SmartUpdate:035a8ee197957396afb827d88b2c5143:cc7b5456ea0723d50293cc819bbf0949:55515e4a88b6dd1be2e6001077894f80$
// ---------------------------------------

#ifndef __PERSONAL_H__
#define __PERSONAL_H__

#include "cocos2d.h"

namespace TexturePacker
{

class Personal
{
public:
    /**
     * Add sprite frames contained in thePersonal sheet to
     * the SpriteFrameCache.
     */
    static void addSpriteFramesToCache();

    /**
     * Remove sprite frames contained in thePersonal sheet from
     * the SpriteFrameCache.
     */
    static void removeSpriteFramesFromCache();

    // ---------------------
    // sprite name constants
    // ---------------------
    static const std::string listSpaceLine;
    static const std::string loginByPhone;
    static const std::string loginByWeChat;
    static const std::string loginByXinlang;
    static const std::string per_touxiang;
    static const std::string redioButtonNormal;
    static const std::string redioButtonSelectedBlue;
    static const std::string redioButtonSelectedOrange;
    static const std::string saveButton;
    static const std::string settingBackground;
    static const std::string settingsContentBackground;
    static const std::string texteditBackground;

    // --------------
    // Sprite objects
    // --------------
    static cocos2d::Sprite *createListSpaceLineSprite();
    static cocos2d::Sprite *createLoginByPhoneSprite();
    static cocos2d::Sprite *createLoginByWeChatSprite();
    static cocos2d::Sprite *createLoginByXinlangSprite();
    static cocos2d::Sprite *createPer_touxiangSprite();
    static cocos2d::Sprite *createRedioButtonNormalSprite();
    static cocos2d::Sprite *createRedioButtonSelectedBlueSprite();
    static cocos2d::Sprite *createRedioButtonSelectedOrangeSprite();
    static cocos2d::Sprite *createSaveButtonSprite();
    static cocos2d::Sprite *createSettingBackgroundSprite();
    static cocos2d::Sprite *createSettingsContentBackgroundSprite();
    static cocos2d::Sprite *createTexteditBackgroundSprite();

}; // class

}; // namespace

#endif // __PERSONAL_H__
