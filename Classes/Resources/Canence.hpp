// ---------------------------------------
// Sprite definitions for Canence
// Generated with TexturePacker 4.8.3
//
// https://www.codeandweb.com/texturepacker
// 
// $TexturePacker:SmartUpdate:d0216283adeea70ad4886e8eb2740235:645e9e10dee7253efe0e921a33825221:5cbd7faffc6c7254526e2fba38027434$
// ---------------------------------------

#ifndef __CANENCE_H__
#define __CANENCE_H__

#include "cocos2d.h"

namespace TexturePacker
{

class Canence
{
public:
    /**
     * Add sprite frames contained in theCanence sheet to
     * the SpriteFrameCache.
     */
    static void addSpriteFramesToCache();

    /**
     * Remove sprite frames contained in theCanence sheet from
     * the SpriteFrameCache.
     */
    static void removeSpriteFramesFromCache();

    // ---------------------
    // sprite name constants
    // ---------------------
    static const std::string canenceBackgroundOne;

    // --------------
    // Sprite objects
    // --------------
    static cocos2d::Sprite *createCanenceBackgroundOneSprite();

}; // class

}; // namespace

#endif // __CANENCE_H__
