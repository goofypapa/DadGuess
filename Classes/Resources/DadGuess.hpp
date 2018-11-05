// ---------------------------------------
// Sprite definitions for DadGuess
// Generated with TexturePacker 4.9.0
//
// https://www.codeandweb.com/texturepacker
// 
// $TexturePacker:SmartUpdate:bc4c6cf5b57dfaf8f351c7169d9cbc35:514bc81b27a7cea960ebcd61366f1e96:396dd5823a2560af3ed645cd9c0391ef$
// ---------------------------------------

#ifndef __DADGUESS_H__
#define __DADGUESS_H__

#include "cocos2d.h"

namespace TexturePacker
{

class DadGuess
{
public:
    /**
     * Add sprite frames contained in theDadGuess sheet to
     * the SpriteFrameCache.
     */
    static void addSpriteFramesToCache();

    /**
     * Remove sprite frames contained in theDadGuess sheet from
     * the SpriteFrameCache.
     */
    static void removeSpriteFramesFromCache();

    // ---------------------
    // sprite name constants
    // ---------------------
    static const std::string dot;
    static const std::string eye;
    static const std::string head;

    // --------------
    // Sprite objects
    // --------------
    static cocos2d::Sprite *createDotSprite();
    static cocos2d::Sprite *createEyeSprite();
    static cocos2d::Sprite *createHeadSprite();

}; // class

}; // namespace

#endif // __DADGUESS_H__
