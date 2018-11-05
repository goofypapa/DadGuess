// ---------------------------------------
// Sprite definitions for PianoGame
// Generated with TexturePacker 4.9.0
//
// https://www.codeandweb.com/texturepacker
// ---------------------------------------

#include "PianoGame.hpp"

USING_NS_CC;

namespace TexturePacker
{

void PianoGame::addSpriteFramesToCache()
{
    SpriteFrameCache *cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("PianoGame.plist");
};

void PianoGame::removeSpriteFramesFromCache()
{
    SpriteFrameCache *cache = SpriteFrameCache::getInstance();
    cache->removeSpriteFramesFromFile("PianoGame.plist");
};


// ---------------------
// sprite name constants
// ---------------------
const std::string PianoGame::centerControl          = "centerControl.png";
const std::string PianoGame::countDown_1            = "countDown_1.png";
const std::string PianoGame::countDown_2            = "countDown_2.png";
const std::string PianoGame::countDown_3            = "countDown_3.png";
const std::string PianoGame::excellent              = "excellent.png";
const std::string PianoGame::home                   = "home.png";
const std::string PianoGame::homeActive             = "homeActive.png";
const std::string PianoGame::keyTouch_0             = "keyTouch_0.png";
const std::string PianoGame::keyTouch_1             = "keyTouch_1.png";
const std::string PianoGame::keyTouch_2             = "keyTouch_2.png";
const std::string PianoGame::keyTouch_3             = "keyTouch_3.png";
const std::string PianoGame::keyTouch_4             = "keyTouch_4.png";
const std::string PianoGame::keyTouch_5             = "keyTouch_5.png";
const std::string PianoGame::keyTouch_6             = "keyTouch_6.png";
const std::string PianoGame::keyTouch_7             = "keyTouch_7.png";
const std::string PianoGame::key_0                  = "key_0.png";
const std::string PianoGame::key_1                  = "key_1.png";
const std::string PianoGame::key_2                  = "key_2.png";
const std::string PianoGame::key_3                  = "key_3.png";
const std::string PianoGame::key_4                  = "key_4.png";
const std::string PianoGame::key_5                  = "key_5.png";
const std::string PianoGame::key_6                  = "key_6.png";
const std::string PianoGame::key_7                  = "key_7.png";
const std::string PianoGame::leak                   = "leak.png";
const std::string PianoGame::mark                   = "mark.png";
const std::string PianoGame::minus                  = "minus.png";
const std::string PianoGame::minusActive            = "minusActive.png";
const std::string PianoGame::musicList              = "musicList.png";
const std::string PianoGame::musicListActive        = "musicListActive.png";
const std::string PianoGame::musicListBackground    = "musicListBackground.png";
const std::string PianoGame::musicListButtonActive  = "musicListButtonActive.png";
const std::string PianoGame::musicListButtonNormal  = "musicListButtonNormal.png";
const std::string PianoGame::musicListButtonTouched = "musicListButtonTouched.png";
const std::string PianoGame::musicListOkButton      = "musicListOkButton.png";
const std::string PianoGame::musicListSelected      = "musicListSelected.png";
const std::string PianoGame::perfect                = "perfect.png";
const std::string PianoGame::play                   = "play.png";
const std::string PianoGame::playStatePause         = "playStatePause.png";
const std::string PianoGame::playStatePauseActive   = "playStatePauseActive.png";
const std::string PianoGame::playStatePlay          = "playStatePlay.png";
const std::string PianoGame::playStatePlayActive    = "playStatePlayActive.png";
const std::string PianoGame::plus                   = "plus.png";
const std::string PianoGame::plusActive             = "plusActive.png";
const std::string PianoGame::shadow                 = "shadow.png";

// ---------------------------------------------------------
// convenience functions returing pointers to Sprite objects
// ---------------------------------------------------------
Sprite* PianoGame::createCenterControlSprite()
{
    return Sprite::createWithSpriteFrameName(centerControl);
}

Sprite* PianoGame::createCountDown_1Sprite()
{
    return Sprite::createWithSpriteFrameName(countDown_1);
}

Sprite* PianoGame::createCountDown_2Sprite()
{
    return Sprite::createWithSpriteFrameName(countDown_2);
}

Sprite* PianoGame::createCountDown_3Sprite()
{
    return Sprite::createWithSpriteFrameName(countDown_3);
}

Sprite* PianoGame::createExcellentSprite()
{
    return Sprite::createWithSpriteFrameName(excellent);
}

Sprite* PianoGame::createHomeSprite()
{
    return Sprite::createWithSpriteFrameName(home);
}

Sprite* PianoGame::createHomeActiveSprite()
{
    return Sprite::createWithSpriteFrameName(homeActive);
}

Sprite* PianoGame::createKeyTouch_0Sprite()
{
    return Sprite::createWithSpriteFrameName(keyTouch_0);
}

Sprite* PianoGame::createKeyTouch_1Sprite()
{
    return Sprite::createWithSpriteFrameName(keyTouch_1);
}

Sprite* PianoGame::createKeyTouch_2Sprite()
{
    return Sprite::createWithSpriteFrameName(keyTouch_2);
}

Sprite* PianoGame::createKeyTouch_3Sprite()
{
    return Sprite::createWithSpriteFrameName(keyTouch_3);
}

Sprite* PianoGame::createKeyTouch_4Sprite()
{
    return Sprite::createWithSpriteFrameName(keyTouch_4);
}

Sprite* PianoGame::createKeyTouch_5Sprite()
{
    return Sprite::createWithSpriteFrameName(keyTouch_5);
}

Sprite* PianoGame::createKeyTouch_6Sprite()
{
    return Sprite::createWithSpriteFrameName(keyTouch_6);
}

Sprite* PianoGame::createKeyTouch_7Sprite()
{
    return Sprite::createWithSpriteFrameName(keyTouch_7);
}

Sprite* PianoGame::createKey_0Sprite()
{
    return Sprite::createWithSpriteFrameName(key_0);
}

Sprite* PianoGame::createKey_1Sprite()
{
    return Sprite::createWithSpriteFrameName(key_1);
}

Sprite* PianoGame::createKey_2Sprite()
{
    return Sprite::createWithSpriteFrameName(key_2);
}

Sprite* PianoGame::createKey_3Sprite()
{
    return Sprite::createWithSpriteFrameName(key_3);
}

Sprite* PianoGame::createKey_4Sprite()
{
    return Sprite::createWithSpriteFrameName(key_4);
}

Sprite* PianoGame::createKey_5Sprite()
{
    return Sprite::createWithSpriteFrameName(key_5);
}

Sprite* PianoGame::createKey_6Sprite()
{
    return Sprite::createWithSpriteFrameName(key_6);
}

Sprite* PianoGame::createKey_7Sprite()
{
    return Sprite::createWithSpriteFrameName(key_7);
}

Sprite* PianoGame::createLeakSprite()
{
    return Sprite::createWithSpriteFrameName(leak);
}

Sprite* PianoGame::createMarkSprite()
{
    return Sprite::createWithSpriteFrameName(mark);
}

Sprite* PianoGame::createMinusSprite()
{
    return Sprite::createWithSpriteFrameName(minus);
}

Sprite* PianoGame::createMinusActiveSprite()
{
    return Sprite::createWithSpriteFrameName(minusActive);
}

Sprite* PianoGame::createMusicListSprite()
{
    return Sprite::createWithSpriteFrameName(musicList);
}

Sprite* PianoGame::createMusicListActiveSprite()
{
    return Sprite::createWithSpriteFrameName(musicListActive);
}

Sprite* PianoGame::createMusicListBackgroundSprite()
{
    return Sprite::createWithSpriteFrameName(musicListBackground);
}

Sprite* PianoGame::createMusicListButtonActiveSprite()
{
    return Sprite::createWithSpriteFrameName(musicListButtonActive);
}

Sprite* PianoGame::createMusicListButtonNormalSprite()
{
    return Sprite::createWithSpriteFrameName(musicListButtonNormal);
}

Sprite* PianoGame::createMusicListButtonTouchedSprite()
{
    return Sprite::createWithSpriteFrameName(musicListButtonTouched);
}

Sprite* PianoGame::createMusicListOkButtonSprite()
{
    return Sprite::createWithSpriteFrameName(musicListOkButton);
}

Sprite* PianoGame::createMusicListSelectedSprite()
{
    return Sprite::createWithSpriteFrameName(musicListSelected);
}

Sprite* PianoGame::createPerfectSprite()
{
    return Sprite::createWithSpriteFrameName(perfect);
}

Sprite* PianoGame::createPlaySprite()
{
    return Sprite::createWithSpriteFrameName(play);
}

Sprite* PianoGame::createPlayStatePauseSprite()
{
    return Sprite::createWithSpriteFrameName(playStatePause);
}

Sprite* PianoGame::createPlayStatePauseActiveSprite()
{
    return Sprite::createWithSpriteFrameName(playStatePauseActive);
}

Sprite* PianoGame::createPlayStatePlaySprite()
{
    return Sprite::createWithSpriteFrameName(playStatePlay);
}

Sprite* PianoGame::createPlayStatePlayActiveSprite()
{
    return Sprite::createWithSpriteFrameName(playStatePlayActive);
}

Sprite* PianoGame::createPlusSprite()
{
    return Sprite::createWithSpriteFrameName(plus);
}

Sprite* PianoGame::createPlusActiveSprite()
{
    return Sprite::createWithSpriteFrameName(plusActive);
}

Sprite* PianoGame::createShadowSprite()
{
    return Sprite::createWithSpriteFrameName(shadow);
}


// ------------------------------------------------
// convenience functions returning animation frames
// ------------------------------------------------

Vector<SpriteFrame*> PianoGame::getCOUNTDOWNAnimationFrames()
{
    SpriteFrameCache *frameCache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame*> frames;
    frames.reserve(3);
    frames.pushBack(frameCache->getSpriteFrameByName(countDown_1));
    frames.pushBack(frameCache->getSpriteFrameByName(countDown_2));
    frames.pushBack(frameCache->getSpriteFrameByName(countDown_3));
    return frames;
}

Vector<SpriteFrame*> PianoGame::getKEYAnimationFrames()
{
    SpriteFrameCache *frameCache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame*> frames;
    frames.reserve(8);
    frames.pushBack(frameCache->getSpriteFrameByName(key_0));
    frames.pushBack(frameCache->getSpriteFrameByName(key_1));
    frames.pushBack(frameCache->getSpriteFrameByName(key_2));
    frames.pushBack(frameCache->getSpriteFrameByName(key_3));
    frames.pushBack(frameCache->getSpriteFrameByName(key_4));
    frames.pushBack(frameCache->getSpriteFrameByName(key_5));
    frames.pushBack(frameCache->getSpriteFrameByName(key_6));
    frames.pushBack(frameCache->getSpriteFrameByName(key_7));
    return frames;
}

Vector<SpriteFrame*> PianoGame::getKEYTOUCHAnimationFrames()
{
    SpriteFrameCache *frameCache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame*> frames;
    frames.reserve(8);
    frames.pushBack(frameCache->getSpriteFrameByName(keyTouch_0));
    frames.pushBack(frameCache->getSpriteFrameByName(keyTouch_1));
    frames.pushBack(frameCache->getSpriteFrameByName(keyTouch_2));
    frames.pushBack(frameCache->getSpriteFrameByName(keyTouch_3));
    frames.pushBack(frameCache->getSpriteFrameByName(keyTouch_4));
    frames.pushBack(frameCache->getSpriteFrameByName(keyTouch_5));
    frames.pushBack(frameCache->getSpriteFrameByName(keyTouch_6));
    frames.pushBack(frameCache->getSpriteFrameByName(keyTouch_7));
    return frames;
}


// ----------
// animations
// ----------

Animate *PianoGame::createCOUNTDOWNAnimateAction(float delay, unsigned int loops)
{
    return Animate::create(Animation::createWithSpriteFrames(getCOUNTDOWNAnimationFrames(), delay, loops));
}
Animate *PianoGame::createKEYAnimateAction(float delay, unsigned int loops)
{
    return Animate::create(Animation::createWithSpriteFrames(getKEYAnimationFrames(), delay, loops));
}
Animate *PianoGame::createKEYTOUCHAnimateAction(float delay, unsigned int loops)
{
    return Animate::create(Animation::createWithSpriteFrames(getKEYTOUCHAnimationFrames(), delay, loops));
}

}; // namespace

