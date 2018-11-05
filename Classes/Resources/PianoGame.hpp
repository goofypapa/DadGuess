// ---------------------------------------
// Sprite definitions for PianoGame
// Generated with TexturePacker 4.9.0
//
// https://www.codeandweb.com/texturepacker
// 
// $TexturePacker:SmartUpdate:c48c0acc67c99fced2b3061a96233c79:0ec5ca2ec318a45093d2f3c48e1ac93d:99752702a6aba31152f2364337dc8068$
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
    static const std::string countDown_1;
    static const std::string countDown_2;
    static const std::string countDown_3;
    static const std::string excellent;
    static const std::string home;
    static const std::string homeActive;
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
    static const std::string leak;
    static const std::string mark;
    static const std::string minus;
    static const std::string minusActive;
    static const std::string musicList;
    static const std::string musicListActive;
    static const std::string musicListBackground;
    static const std::string musicListButtonActive;
    static const std::string musicListButtonNormal;
    static const std::string musicListButtonTouched;
    static const std::string musicListOkButton;
    static const std::string musicListSelected;
    static const std::string perfect;
    static const std::string play;
    static const std::string playStatePause;
    static const std::string playStatePauseActive;
    static const std::string playStatePlay;
    static const std::string playStatePlayActive;
    static const std::string plus;
    static const std::string plusActive;
    static const std::string shadow;

    // --------------
    // Sprite objects
    // --------------
    static cocos2d::Sprite *createCenterControlSprite();
    static cocos2d::Sprite *createCountDown_1Sprite();
    static cocos2d::Sprite *createCountDown_2Sprite();
    static cocos2d::Sprite *createCountDown_3Sprite();
    static cocos2d::Sprite *createExcellentSprite();
    static cocos2d::Sprite *createHomeSprite();
    static cocos2d::Sprite *createHomeActiveSprite();
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
    static cocos2d::Sprite *createLeakSprite();
    static cocos2d::Sprite *createMarkSprite();
    static cocos2d::Sprite *createMinusSprite();
    static cocos2d::Sprite *createMinusActiveSprite();
    static cocos2d::Sprite *createMusicListSprite();
    static cocos2d::Sprite *createMusicListActiveSprite();
    static cocos2d::Sprite *createMusicListBackgroundSprite();
    static cocos2d::Sprite *createMusicListButtonActiveSprite();
    static cocos2d::Sprite *createMusicListButtonNormalSprite();
    static cocos2d::Sprite *createMusicListButtonTouchedSprite();
    static cocos2d::Sprite *createMusicListOkButtonSprite();
    static cocos2d::Sprite *createMusicListSelectedSprite();
    static cocos2d::Sprite *createPerfectSprite();
    static cocos2d::Sprite *createPlaySprite();
    static cocos2d::Sprite *createPlayStatePauseSprite();
    static cocos2d::Sprite *createPlayStatePauseActiveSprite();
    static cocos2d::Sprite *createPlayStatePlaySprite();
    static cocos2d::Sprite *createPlayStatePlayActiveSprite();
    static cocos2d::Sprite *createPlusSprite();
    static cocos2d::Sprite *createPlusActiveSprite();
    static cocos2d::Sprite *createShadowSprite();

    // ----------------
    // animation frames
    // ----------------
    static cocos2d::Vector<cocos2d::SpriteFrame*> getCOUNTDOWNAnimationFrames();
    static cocos2d::Vector<cocos2d::SpriteFrame*> getKEYAnimationFrames();
    static cocos2d::Vector<cocos2d::SpriteFrame*> getKEYTOUCHAnimationFrames();

    // ---------------
    // animate actions
    // ---------------
    static cocos2d::Animate *createCOUNTDOWNAnimateAction(float delay, unsigned int loops = 1);
    static cocos2d::Animate *createKEYAnimateAction(float delay, unsigned int loops = 1);
    static cocos2d::Animate *createKEYTOUCHAnimateAction(float delay, unsigned int loops = 1);

}; // class

}; // namespace

#endif // __PIANOGAME_H__
