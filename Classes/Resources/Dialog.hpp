// ---------------------------------------
// Sprite definitions for Dialog
// Generated with TexturePacker 4.8.3
//
// https://www.codeandweb.com/texturepacker
// 
// $TexturePacker:SmartUpdate:eb5a6df173df219c7877f701f33b5726:cbf55cbe15e32da0bed98dc0e91fc1cf:9d5f9d777bce4db9fed050e341ff8596$
// ---------------------------------------

#ifndef __DIALOG_H__
#define __DIALOG_H__

#include "cocos2d.h"

namespace TexturePacker
{

class Dialog
{
public:
    /**
     * Add sprite frames contained in theDialog sheet to
     * the SpriteFrameCache.
     */
    static void addSpriteFramesToCache();

    /**
     * Remove sprite frames contained in theDialog sheet from
     * the SpriteFrameCache.
     */
    static void removeSpriteFramesFromCache();

    // ---------------------
    // sprite name constants
    // ---------------------
    static const std::string confirmButton;
    static const std::string listSpaceLine;
    static const std::string loginByPhone;
    static const std::string loginByWeChat;
    static const std::string loginByXinlang;
    static const std::string per_touxiang;
    static const std::string personalCenterBackground;
    static const std::string redioButtonNormal;
    static const std::string redioButtonSelectedBlue;
    static const std::string redioButtonSelectedOrange;
    static const std::string saveButton;
    static const std::string settingBackground;
    static const std::string settingsContentBackground;
    static const std::string swichEnabled;
    static const std::string swichNormal;
    static const std::string texteditBackground;

    // --------------
    // Sprite objects
    // --------------
    static cocos2d::Sprite *createConfirmButtonSprite();
    static cocos2d::Sprite *createListSpaceLineSprite();
    static cocos2d::Sprite *createLoginByPhoneSprite();
    static cocos2d::Sprite *createLoginByWeChatSprite();
    static cocos2d::Sprite *createLoginByXinlangSprite();
    static cocos2d::Sprite *createPer_touxiangSprite();
    static cocos2d::Sprite *createPersonalCenterBackgroundSprite();
    static cocos2d::Sprite *createRedioButtonNormalSprite();
    static cocos2d::Sprite *createRedioButtonSelectedBlueSprite();
    static cocos2d::Sprite *createRedioButtonSelectedOrangeSprite();
    static cocos2d::Sprite *createSaveButtonSprite();
    static cocos2d::Sprite *createSettingBackgroundSprite();
    static cocos2d::Sprite *createSettingsContentBackgroundSprite();
    static cocos2d::Sprite *createSwichEnabledSprite();
    static cocos2d::Sprite *createSwichNormalSprite();
    static cocos2d::Sprite *createTexteditBackgroundSprite();

}; // class

}; // namespace

#endif // __DIALOG_H__
