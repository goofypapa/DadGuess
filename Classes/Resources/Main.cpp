// ---------------------------------------
// Sprite definitions for Main
// Generated with TexturePacker 4.8.3
//
// https://www.codeandweb.com/texturepacker
// ---------------------------------------

#include "Main.hpp"

USING_NS_CC;

namespace TexturePacker
{

void Main::addSpriteFramesToCache()
{
    SpriteFrameCache *cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("Main.plist");
};

void Main::removeSpriteFramesFromCache()
{
    SpriteFrameCache *cache = SpriteFrameCache::getInstance();
    cache->removeSpriteFramesFromFile("Main.plist");
};


// ---------------------
// sprite name constants
// ---------------------
const std::string Main::mainBaike              = "MainBaike.png";
const std::string Main::mainDadpat             = "MainDadpat.png";
const std::string Main::mainGame               = "MainGame.png";
const std::string Main::mainMessage            = "MainMessage.png";
const std::string Main::mainNewMessage         = "MainNewMessage.png";
const std::string Main::mainNotice             = "MainNotice.png";
const std::string Main::mainSetting            = "MainSetting.png";
const std::string Main::mainUserHeadBackground = "MainUserHeadBackground.png";

// ---------------------------------------------------------
// convenience functions returing pointers to Sprite objects
// ---------------------------------------------------------
Sprite* Main::createMainBaikeSprite()
{
    return Sprite::createWithSpriteFrameName(mainBaike);
}

Sprite* Main::createMainDadpatSprite()
{
    return Sprite::createWithSpriteFrameName(mainDadpat);
}

Sprite* Main::createMainGameSprite()
{
    return Sprite::createWithSpriteFrameName(mainGame);
}

Sprite* Main::createMainMessageSprite()
{
    return Sprite::createWithSpriteFrameName(mainMessage);
}

Sprite* Main::createMainNewMessageSprite()
{
    return Sprite::createWithSpriteFrameName(mainNewMessage);
}

Sprite* Main::createMainNoticeSprite()
{
    return Sprite::createWithSpriteFrameName(mainNotice);
}

Sprite* Main::createMainSettingSprite()
{
    return Sprite::createWithSpriteFrameName(mainSetting);
}

Sprite* Main::createMainUserHeadBackgroundSprite()
{
    return Sprite::createWithSpriteFrameName(mainUserHeadBackground);
}


}; // namespace

