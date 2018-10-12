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
const std::string Main::mainIcoABC                = "MainIcoABC.png";
const std::string Main::mainIcoAnimalEncyclopedia = "MainIcoAnimalEncyclopedia.png";
const std::string Main::mainIcoAstronomy          = "MainIcoAstronomy.png";
const std::string Main::mainIcoChineseHistory     = "MainIcoChineseHistory.png";
const std::string Main::mainIcoDadGuess           = "MainIcoDadGuess.png";
const std::string Main::mainIcoDadpat             = "MainIcoDadpat.png";
const std::string Main::mainIcoEarth              = "MainIcoEarth.png";
const std::string Main::mainIcoFamousPainting     = "MainIcoFamousPainting.png";
const std::string Main::mainIcoHistoryOfTheWorld  = "MainIcoHistoryOfTheWorld.png";
const std::string Main::mainIcoMonth              = "MainIcoMonth.png";
const std::string Main::mainIcoPiano              = "MainIcoPiano.png";
const std::string Main::mainIcoRhythmGame         = "MainIcoRhythmGame.png";
const std::string Main::mainMessage               = "MainMessage.png";
const std::string Main::mainNewMessage            = "MainNewMessage.png";
const std::string Main::mainNotice                = "MainNotice.png";
const std::string Main::mainSetting               = "MainSetting.png";
const std::string Main::mainUserHeadBackground    = "MainUserHeadBackground.png";

// ---------------------------------------------------------
// convenience functions returing pointers to Sprite objects
// ---------------------------------------------------------
Sprite* Main::createMainIcoABCSprite()
{
    return Sprite::createWithSpriteFrameName(mainIcoABC);
}

Sprite* Main::createMainIcoAnimalEncyclopediaSprite()
{
    return Sprite::createWithSpriteFrameName(mainIcoAnimalEncyclopedia);
}

Sprite* Main::createMainIcoAstronomySprite()
{
    return Sprite::createWithSpriteFrameName(mainIcoAstronomy);
}

Sprite* Main::createMainIcoChineseHistorySprite()
{
    return Sprite::createWithSpriteFrameName(mainIcoChineseHistory);
}

Sprite* Main::createMainIcoDadGuessSprite()
{
    return Sprite::createWithSpriteFrameName(mainIcoDadGuess);
}

Sprite* Main::createMainIcoDadpatSprite()
{
    return Sprite::createWithSpriteFrameName(mainIcoDadpat);
}

Sprite* Main::createMainIcoEarthSprite()
{
    return Sprite::createWithSpriteFrameName(mainIcoEarth);
}

Sprite* Main::createMainIcoFamousPaintingSprite()
{
    return Sprite::createWithSpriteFrameName(mainIcoFamousPainting);
}

Sprite* Main::createMainIcoHistoryOfTheWorldSprite()
{
    return Sprite::createWithSpriteFrameName(mainIcoHistoryOfTheWorld);
}

Sprite* Main::createMainIcoMonthSprite()
{
    return Sprite::createWithSpriteFrameName(mainIcoMonth);
}

Sprite* Main::createMainIcoPianoSprite()
{
    return Sprite::createWithSpriteFrameName(mainIcoPiano);
}

Sprite* Main::createMainIcoRhythmGameSprite()
{
    return Sprite::createWithSpriteFrameName(mainIcoRhythmGame);
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

