// ---------------------------------------
// Sprite definitions for Canence
// Generated with TexturePacker 4.8.3
//
// https://www.codeandweb.com/texturepacker
// ---------------------------------------

#include "Canence.hpp"

USING_NS_CC;

namespace TexturePacker
{

void Canence::addSpriteFramesToCache()
{
    SpriteFrameCache *cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("Canence.plist");
};

void Canence::removeSpriteFramesFromCache()
{
    SpriteFrameCache *cache = SpriteFrameCache::getInstance();
    cache->removeSpriteFramesFromFile("Canence.plist");
};


// ---------------------
// sprite name constants
// ---------------------
const std::string Canence::canenceBackgroundOne = "CanenceBackgroundOne.png";

// ---------------------------------------------------------
// convenience functions returing pointers to Sprite objects
// ---------------------------------------------------------
Sprite* Canence::createCanenceBackgroundOneSprite()
{
    return Sprite::createWithSpriteFrameName(canenceBackgroundOne);
}


}; // namespace

