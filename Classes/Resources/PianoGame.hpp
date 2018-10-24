// ---------------------------------------
// Sprite definitions for PianoGame
// Generated with TexturePacker 4.9.0
//
// https://www.codeandweb.com/texturepacker
// 
// $TexturePacker:SmartUpdate:ad98340d8936f6447132ef484af2a44c:b61c89b286a7592008449f0fa9670b9c:99752702a6aba31152f2364337dc8068$
// ---------------------------------------

#ifndef __PIANOGAME_H__
#define __PIANOGAME_H__

#include "cocos2d.h"

namespace TexturePacker
{

class PianoGame
{
public:
    /**
     * Add sprite frames contained in thePianoGame sheet to
     * the SpriteFrameCache.
     */
    static void addSpriteFramesToCache();

    /**
     * Remove sprite frames contained in thePianoGame sheet from
     * the SpriteFrameCache.
     */
    static void removeSpriteFramesFromCache();

    // ---------------------
    // sprite name constants
    // ---------------------
    static const std::string centerControl;
    static const std::string keyTouch_0;
    static const std::string keyTouch_1;
    static const std::string keyTouch_2;
    static const std::string keyTouch_3;
    static const std::string keyTouch_4;
    static const std::string keyTouch_5;
    static const std::string keyTouch_6;
    static const std::string keyTouch_7;
    static const std::string key_0;
    static const std::string key_1;
    static const std::string key_2;
    static const std::string key_3;
    static const std::string key_4;
    static const std::string key_5;
    static const std::string key_6;
    static const std::string key_7;
    static const std::string mark;
    static const std::string minus;
    static const std::string plus;
    static const std::string shadow;

    // --------------
    // Sprite objects
    // --------------
    static cocos2d::Sprite *createCenterControlSprite();
    static cocos2d::Sprite *createKeyTouch_0Sprite();
    static cocos2d::Sprite *createKeyTouch_1Sprite();
    static cocos2d::Sprite *createKeyTouch_2Sprite();
    static cocos2d::Sprite *createKeyTouch_3Sprite();
    static cocos2d::Sprite *createKeyTouch_4Sprite();
    static cocos2d::Sprite *createKeyTouch_5Sprite();
    static cocos2d::Sprite *createKeyTouch_6Sprite();
    static cocos2d::Sprite *createKeyTouch_7Sprite();
    static cocos2d::Sprite *createKey_0Sprite();
    static cocos2d::Sprite *createKey_1Sprite();
    static cocos2d::Sprite *createKey_2Sprite();
    static cocos2d::Sprite *createKey_3Sprite();
    static cocos2d::Sprite *createKey_4Sprite();
    static cocos2d::Sprite *createKey_5Sprite();
    static cocos2d::Sprite *createKey_6Sprite();
    static cocos2d::Sprite *createKey_7Sprite();
    static cocos2d::Sprite *createMarkSprite();
    static cocos2d::Sprite *createMinusSprite();
    static cocos2d::Sprite *createPlusSprite();
    static cocos2d::Sprite *createShadowSprite();

    // ----------------
    // animation frames
    // ----------------
    static cocos2d::Vector<cocos2d::SpriteFrame*> getKEYAnimationFrames();
    static cocos2d::Vector<cocos2d::SpriteFrame*> getKEYTOUCHAnimationFrames();

    // ---------------
    // animate actions
    // ---------------
    static cocos2d::Animate *createKEYAnimateAction(float delay, unsigned int loops = 1);
    static cocos2d::Animate *createKEYTOUCHAnimateAction(float delay, unsigned int loops = 1);

}; // class

}; // namespace

#endif // __PIANOGAME_H__
