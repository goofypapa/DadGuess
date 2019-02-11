// ---------------------------------------
// Sprite definitions for DadGuessUpdate
// Generated with TexturePacker 4.11.1
//
// https://www.codeandweb.com/texturepacker
// ---------------------------------------

#include "DadGuessUpdate.hpp"

USING_NS_CC;

namespace TexturePacker
{

void DadGuessUpdate::addSpriteFramesToCache()
{
    SpriteFrameCache *cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("DadGuessUpdate.plist");
};

void DadGuessUpdate::removeSpriteFramesFromCache()
{
    SpriteFrameCache *cache = SpriteFrameCache::getInstance();
    cache->removeSpriteFramesFromFile("DadGuessUpdate.plist");
};


// ---------------------
// sprite name constants
// ---------------------
const std::string DadGuessUpdate::button                   = "Button.png";
const std::string DadGuessUpdate::caicai_update_background = "caicai_update_background.png";

// ---------------------------------------------------------
// convenience functions returing pointers to Sprite objects
// ---------------------------------------------------------
Sprite* DadGuessUpdate::createButtonSprite()
{
    return Sprite::createWithSpriteFrameName(button);
}

Sprite* DadGuessUpdate::createCaicai_update_backgroundSprite()
{
    return Sprite::createWithSpriteFrameName(caicai_update_background);
}


}; // namespace

