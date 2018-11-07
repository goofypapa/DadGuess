// ---------------------------------------
// Sprite definitions for CadenceMain
// Generated with TexturePacker 4.9.0
//
// https://www.codeandweb.com/texturepacker
// 
// $TexturePacker:SmartUpdate:c328cae82c1d5125eb3e85f6e36d99c6:7257a9154d7d75238f27c164182cb006:6dfe92147b7e8a4cfe024ace3bafa3fc$
// ---------------------------------------

#ifndef __CADENCEMAIN_H__
#define __CADENCEMAIN_H__

#include "cocos2d.h"

namespace TexturePacker
{

class CadenceMain
{
public:
    /**
     * Add sprite frames contained in theCadenceMain sheet to
     * the SpriteFrameCache.
     */
    static void addSpriteFramesToCache();

    /**
     * Remove sprite frames contained in theCadenceMain sheet from
     * the SpriteFrameCache.
     */
    static void removeSpriteFramesFromCache();

    // ---------------------
    // sprite name constants
    // ---------------------
    static const std::string cadenceBackground_1;
    static const std::string cadenceBackground_2;
    static const std::string cadenceBackground_3;
    static const std::string cadenceBackground_4;

    // --------------
    // Sprite objects
    // --------------
    static cocos2d::Sprite *createCadenceBackground_1Sprite();
    static cocos2d::Sprite *createCadenceBackground_2Sprite();
    static cocos2d::Sprite *createCadenceBackground_3Sprite();
    static cocos2d::Sprite *createCadenceBackground_4Sprite();

    // ----------------
    // animation frames
    // ----------------
    static cocos2d::Vector<cocos2d::SpriteFrame*> getCADENCEBACKGROUNDAnimationFrames();

    // ---------------
    // animate actions
    // ---------------
    static cocos2d::Animate *createCADENCEBACKGROUNDAnimateAction(float delay, unsigned int loops = 1);

}; // class

}; // namespace

#endif // __CADENCEMAIN_H__
