// ---------------------------------------
// Sprite definitions for DadGuess
// Generated with TexturePacker 4.9.0
//
// https://www.codeandweb.com/texturepacker
// ---------------------------------------

#include "DadGuess.hpp"

USING_NS_CC;

namespace TexturePacker
{

void DadGuess::addSpriteFramesToCache()
{
    SpriteFrameCache *cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("DadGuess.plist");
};

void DadGuess::removeSpriteFramesFromCache()
{
    SpriteFrameCache *cache = SpriteFrameCache::getInstance();
    cache->removeSpriteFramesFromFile("DadGuess.plist");
};


// ---------------------
// sprite name constants
// ---------------------
const std::string DadGuess::dot  = "dot.png";
const std::string DadGuess::eye  = "eye.png";
const std::string DadGuess::head = "head.png";

// ---------------------------------------------------------
// convenience functions returing pointers to Sprite objects
// ---------------------------------------------------------
Sprite* DadGuess::createDotSprite()
{
    return Sprite::createWithSpriteFrameName(dot);
}

Sprite* DadGuess::createEyeSprite()
{
    return Sprite::createWithSpriteFrameName(eye);
}

Sprite* DadGuess::createHeadSprite()
{
    return Sprite::createWithSpriteFrameName(head);
}


}; // namespace

