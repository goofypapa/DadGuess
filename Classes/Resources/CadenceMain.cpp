// ---------------------------------------
// Sprite definitions for CadenceMain
// Generated with TexturePacker 4.9.0
//
// https://www.codeandweb.com/texturepacker
// ---------------------------------------

#include "CadenceMain.hpp"

USING_NS_CC;

namespace TexturePacker
{

void CadenceMain::addSpriteFramesToCache()
{
    SpriteFrameCache *cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("CadenceMain.plist");
};

void CadenceMain::removeSpriteFramesFromCache()
{
    SpriteFrameCache *cache = SpriteFrameCache::getInstance();
    cache->removeSpriteFramesFromFile("CadenceMain.plist");
};


// ---------------------
// sprite name constants
// ---------------------
const std::string CadenceMain::cadenceBackground_1 = "CadenceBackground_1.png";
const std::string CadenceMain::cadenceBackground_2 = "CadenceBackground_2.png";
const std::string CadenceMain::cadenceBackground_3 = "CadenceBackground_3.png";
const std::string CadenceMain::cadenceBackground_4 = "CadenceBackground_4.png";

// ---------------------------------------------------------
// convenience functions returing pointers to Sprite objects
// ---------------------------------------------------------
Sprite* CadenceMain::createCadenceBackground_1Sprite()
{
    return Sprite::createWithSpriteFrameName(cadenceBackground_1);
}

Sprite* CadenceMain::createCadenceBackground_2Sprite()
{
    return Sprite::createWithSpriteFrameName(cadenceBackground_2);
}

Sprite* CadenceMain::createCadenceBackground_3Sprite()
{
    return Sprite::createWithSpriteFrameName(cadenceBackground_3);
}

Sprite* CadenceMain::createCadenceBackground_4Sprite()
{
    return Sprite::createWithSpriteFrameName(cadenceBackground_4);
}


// ------------------------------------------------
// convenience functions returning animation frames
// ------------------------------------------------

Vector<SpriteFrame*> CadenceMain::getCADENCEBACKGROUNDAnimationFrames()
{
    SpriteFrameCache *frameCache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame*> frames;
    frames.reserve(4);
    frames.pushBack(frameCache->getSpriteFrameByName(cadenceBackground_1));
    frames.pushBack(frameCache->getSpriteFrameByName(cadenceBackground_2));
    frames.pushBack(frameCache->getSpriteFrameByName(cadenceBackground_3));
    frames.pushBack(frameCache->getSpriteFrameByName(cadenceBackground_4));
    return frames;
}


// ----------
// animations
// ----------

Animate *CadenceMain::createCADENCEBACKGROUNDAnimateAction(float delay, unsigned int loops)
{
    return Animate::create(Animation::createWithSpriteFrames(getCADENCEBACKGROUNDAnimationFrames(), delay, loops));
}

}; // namespace

