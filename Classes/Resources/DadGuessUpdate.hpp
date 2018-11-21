// ---------------------------------------
// Sprite definitions for DadGuessUpdate
// Generated with TexturePacker 4.10.0
//
// https://www.codeandweb.com/texturepacker
// 
// $TexturePacker:SmartUpdate:fc00fdeb70dcbbfaafdcaa7decbb3bea:5d66b61d4e72a8e50e4cfe603c476dff:5a3ccc56e0a2abd4cfd9e5161bd302f7$
// ---------------------------------------

#ifndef __DADGUESSUPDATE_H__
#define __DADGUESSUPDATE_H__

#include "cocos2d.h"

namespace TexturePacker
{

class DadGuessUpdate
{
public:
    /**
     * Add sprite frames contained in theDadGuessUpdate sheet to
     * the SpriteFrameCache.
     */
    static void addSpriteFramesToCache();

    /**
     * Remove sprite frames contained in theDadGuessUpdate sheet from
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

#endif // __DADGUESSUPDATE_H__
