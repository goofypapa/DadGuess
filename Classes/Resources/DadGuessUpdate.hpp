// ---------------------------------------
// Sprite definitions for DadGuessUpdate
// Generated with TexturePacker 4.11.1
//
// https://www.codeandweb.com/texturepacker
// 
// $TexturePacker:SmartUpdate:77e6c5a4ade646dabce228d5c07ccb8e:c0bfb28286ee6a172422fb39d649aea3:5a3ccc56e0a2abd4cfd9e5161bd302f7$
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
    static const std::string button;
    static const std::string caicai_update_background;

    // --------------
    // Sprite objects
    // --------------
    static cocos2d::Sprite *createButtonSprite();
    static cocos2d::Sprite *createCaicai_update_backgroundSprite();

}; // class

}; // namespace

#endif // __DADGUESSUPDATE_H__
