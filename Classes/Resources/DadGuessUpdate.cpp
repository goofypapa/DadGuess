// ---------------------------------------
// Sprite definitions for DadGuessUpdate
// Generated with TexturePacker 4.12.0
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
const std::string DadGuessUpdate::caicai_update_background = "caicai_update_background.png";

// ---------------------------------------------------------
// convenience functions returing pointers to Sprite objects
// ---------------------------------------------------------
Sprite* DadGuessUpdate::createCaicai_update_backgroundSprite()
{
    return Sprite::createWithSpriteFrameName(caicai_update_background);
}


}; // namespace

