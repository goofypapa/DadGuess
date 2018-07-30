// ---------------------------------------
// Sprite definitions for Personal
// Generated with TexturePacker 4.8.2
//
// https://www.codeandweb.com/texturepacker
// ---------------------------------------

#include "Personal.hpp"

USING_NS_CC;

namespace TexturePacker
{

void Personal::addSpriteFramesToCache()
{
    SpriteFrameCache *cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("Personal.plist");
};

void Personal::removeSpriteFramesFromCache()
{
    SpriteFrameCache *cache = SpriteFrameCache::getInstance();
    cache->removeSpriteFramesFromFile("Personal.plist");
};


// ---------------------
// sprite name constants
// ---------------------
const std::string Personal::listSpaceLine             = "ListSpaceLine.png";
const std::string Personal::loginByPhone              = "LoginByPhone.png";
const std::string Personal::loginByWeChat             = "LoginByWeChat.png";
const std::string Personal::loginByXinlang            = "LoginByXinlang.png";
const std::string Personal::per_touxiang              = "per_touxiang.png";
const std::string Personal::redioButtonNormal         = "RedioButtonNormal.png";
const std::string Personal::redioButtonSelectedBlue   = "RedioButtonSelectedBlue.png";
const std::string Personal::redioButtonSelectedOrange = "RedioButtonSelectedOrange.png";
const std::string Personal::saveButton                = "SaveButton.png";
const std::string Personal::settingBackground         = "SettingBackground.png";
const std::string Personal::settingsContentBackground = "SettingsContentBackground.png";
const std::string Personal::texteditBackground        = "TexteditBackground.png";

// ---------------------------------------------------------
// convenience functions returing pointers to Sprite objects
// ---------------------------------------------------------
Sprite* Personal::createListSpaceLineSprite()
{
    return Sprite::createWithSpriteFrameName(listSpaceLine);
}

Sprite* Personal::createLoginByPhoneSprite()
{
    return Sprite::createWithSpriteFrameName(loginByPhone);
}

Sprite* Personal::createLoginByWeChatSprite()
{
    return Sprite::createWithSpriteFrameName(loginByWeChat);
}

Sprite* Personal::createLoginByXinlangSprite()
{
    return Sprite::createWithSpriteFrameName(loginByXinlang);
}

Sprite* Personal::createPer_touxiangSprite()
{
    return Sprite::createWithSpriteFrameName(per_touxiang);
}

Sprite* Personal::createRedioButtonNormalSprite()
{
    return Sprite::createWithSpriteFrameName(redioButtonNormal);
}

Sprite* Personal::createRedioButtonSelectedBlueSprite()
{
    return Sprite::createWithSpriteFrameName(redioButtonSelectedBlue);
}

Sprite* Personal::createRedioButtonSelectedOrangeSprite()
{
    return Sprite::createWithSpriteFrameName(redioButtonSelectedOrange);
}

Sprite* Personal::createSaveButtonSprite()
{
    return Sprite::createWithSpriteFrameName(saveButton);
}

Sprite* Personal::createSettingBackgroundSprite()
{
    return Sprite::createWithSpriteFrameName(settingBackground);
}

Sprite* Personal::createSettingsContentBackgroundSprite()
{
    return Sprite::createWithSpriteFrameName(settingsContentBackground);
}

Sprite* Personal::createTexteditBackgroundSprite()
{
    return Sprite::createWithSpriteFrameName(texteditBackground);
}


}; // namespace

