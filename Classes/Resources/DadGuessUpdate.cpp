// ---------------------------------------
// Sprite definitions for DadGuessUpdate
// Generated with TexturePacker 4.10.0
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
const std::string DadGuessUpdate::dot  = "dot.png";
const std::string DadGuessUpdate::eye  = "eye.png";
const std::string DadGuessUpdate::head = "head.png";

// ---------------------------------------------------------
// convenience functions returing pointers to Sprite objects
// ---------------------------------------------------------
Sprite* DadGuessUpdate::createDotSprite()
{
    return Sprite::createWithSpriteFrameName(dot);
}

Sprite* DadGuessUpdate::createEyeSprite()
{
    return Sprite::createWithSpriteFrameName(eye);
}

Sprite* DadGuessUpdate::createHeadSprite()
{
    return Sprite::createWithSpriteFrameName(head);
}


}; // namespace

