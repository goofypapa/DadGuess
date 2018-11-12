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
const std::string CadenceMain::back                         = "back.png";
const std::string CadenceMain::difficulty                   = "difficulty.png";
const std::string CadenceMain::difficulty_selected          = "difficulty_selected.png";
const std::string CadenceMain::game_pic_ball_blue           = "game_pic_ball_blue.png";
const std::string CadenceMain::game_pic_ball_pink           = "game_pic_ball_pink.png";
const std::string CadenceMain::game_pic_ball_yellow         = "game_pic_ball_yellow.png";
const std::string CadenceMain::game_pic_end                 = "game_pic_end.png";
const std::string CadenceMain::game_pic_gu                  = "game_pic_gu.png";
const std::string CadenceMain::game_pic_guide_blue          = "game_pic_guide_blue.png";
const std::string CadenceMain::game_pic_guide_green         = "game_pic_guide_green.png";
const std::string CadenceMain::game_pic_guide_purple        = "game_pic_guide_purple.png";
const std::string CadenceMain::game_pic_guide_remind_pink   = "game_pic_guide_remind_pink.png";
const std::string CadenceMain::game_pic_guide_remind_purple = "game_pic_guide_remind_purple.png";
const std::string CadenceMain::game_pic_guide_remind_yellow = "game_pic_guide_remind_yellow.png";
const std::string CadenceMain::game_pic_guide_yellow        = "game_pic_guide_yellow.png";
const std::string CadenceMain::game_pic_light_center_n      = "game_pic_light_center_n.png";
const std::string CadenceMain::game_pic_light_center_p      = "game_pic_light_center_p.png";
const std::string CadenceMain::game_pic_light_mid_n         = "game_pic_light_mid_n.png";
const std::string CadenceMain::game_pic_light_mid_p         = "game_pic_light_mid_p.png";
const std::string CadenceMain::game_pic_light_out_n         = "game_pic_light_out_n.png";
const std::string CadenceMain::game_pic_light_out_p         = "game_pic_light_out_p.png";
const std::string CadenceMain::good                         = "good.png";
const std::string CadenceMain::miss                         = "miss.png";
const std::string CadenceMain::ordinary                     = "ordinary.png";
const std::string CadenceMain::ordinary_selected            = "ordinary_selected.png";
const std::string CadenceMain::perfect                      = "perfect.png";
const std::string CadenceMain::select_sea_srtar1_s          = "select_sea_srtar1_s.png";
const std::string CadenceMain::simple                       = "simple.png";
const std::string CadenceMain::simple_selected              = "simple_selected.png";
const std::string CadenceMain::soundNameLeft                = "soundNameLeft.png";
const std::string CadenceMain::soundNameRight               = "soundNameRight.png";
const std::string CadenceMain::start                        = "start.png";
const std::string CadenceMain::startButton                  = "startButton.png";

// ---------------------------------------------------------
// convenience functions returing pointers to Sprite objects
// ---------------------------------------------------------
Sprite* CadenceMain::createBackSprite()
{
    return Sprite::createWithSpriteFrameName(back);
}

Sprite* CadenceMain::createDifficultySprite()
{
    return Sprite::createWithSpriteFrameName(difficulty);
}

Sprite* CadenceMain::createDifficulty_selectedSprite()
{
    return Sprite::createWithSpriteFrameName(difficulty_selected);
}

Sprite* CadenceMain::createGame_pic_ball_blueSprite()
{
    return Sprite::createWithSpriteFrameName(game_pic_ball_blue);
}

Sprite* CadenceMain::createGame_pic_ball_pinkSprite()
{
    return Sprite::createWithSpriteFrameName(game_pic_ball_pink);
}

Sprite* CadenceMain::createGame_pic_ball_yellowSprite()
{
    return Sprite::createWithSpriteFrameName(game_pic_ball_yellow);
}

Sprite* CadenceMain::createGame_pic_endSprite()
{
    return Sprite::createWithSpriteFrameName(game_pic_end);
}

Sprite* CadenceMain::createGame_pic_guSprite()
{
    return Sprite::createWithSpriteFrameName(game_pic_gu);
}

Sprite* CadenceMain::createGame_pic_guide_blueSprite()
{
    return Sprite::createWithSpriteFrameName(game_pic_guide_blue);
}

Sprite* CadenceMain::createGame_pic_guide_greenSprite()
{
    return Sprite::createWithSpriteFrameName(game_pic_guide_green);
}

Sprite* CadenceMain::createGame_pic_guide_purpleSprite()
{
    return Sprite::createWithSpriteFrameName(game_pic_guide_purple);
}

Sprite* CadenceMain::createGame_pic_guide_remind_pinkSprite()
{
    return Sprite::createWithSpriteFrameName(game_pic_guide_remind_pink);
}

Sprite* CadenceMain::createGame_pic_guide_remind_purpleSprite()
{
    return Sprite::createWithSpriteFrameName(game_pic_guide_remind_purple);
}

Sprite* CadenceMain::createGame_pic_guide_remind_yellowSprite()
{
    return Sprite::createWithSpriteFrameName(game_pic_guide_remind_yellow);
}

Sprite* CadenceMain::createGame_pic_guide_yellowSprite()
{
    return Sprite::createWithSpriteFrameName(game_pic_guide_yellow);
}

Sprite* CadenceMain::createGame_pic_light_center_nSprite()
{
    return Sprite::createWithSpriteFrameName(game_pic_light_center_n);
}

Sprite* CadenceMain::createGame_pic_light_center_pSprite()
{
    return Sprite::createWithSpriteFrameName(game_pic_light_center_p);
}

Sprite* CadenceMain::createGame_pic_light_mid_nSprite()
{
    return Sprite::createWithSpriteFrameName(game_pic_light_mid_n);
}

Sprite* CadenceMain::createGame_pic_light_mid_pSprite()
{
    return Sprite::createWithSpriteFrameName(game_pic_light_mid_p);
}

Sprite* CadenceMain::createGame_pic_light_out_nSprite()
{
    return Sprite::createWithSpriteFrameName(game_pic_light_out_n);
}

Sprite* CadenceMain::createGame_pic_light_out_pSprite()
{
    return Sprite::createWithSpriteFrameName(game_pic_light_out_p);
}

Sprite* CadenceMain::createGoodSprite()
{
    return Sprite::createWithSpriteFrameName(good);
}

Sprite* CadenceMain::createMissSprite()
{
    return Sprite::createWithSpriteFrameName(miss);
}

Sprite* CadenceMain::createOrdinarySprite()
{
    return Sprite::createWithSpriteFrameName(ordinary);
}

Sprite* CadenceMain::createOrdinary_selectedSprite()
{
    return Sprite::createWithSpriteFrameName(ordinary_selected);
}

Sprite* CadenceMain::createPerfectSprite()
{
    return Sprite::createWithSpriteFrameName(perfect);
}

Sprite* CadenceMain::createSelect_sea_srtar1_sSprite()
{
    return Sprite::createWithSpriteFrameName(select_sea_srtar1_s);
}

Sprite* CadenceMain::createSimpleSprite()
{
    return Sprite::createWithSpriteFrameName(simple);
}

Sprite* CadenceMain::createSimple_selectedSprite()
{
    return Sprite::createWithSpriteFrameName(simple_selected);
}

Sprite* CadenceMain::createSoundNameLeftSprite()
{
    return Sprite::createWithSpriteFrameName(soundNameLeft);
}

Sprite* CadenceMain::createSoundNameRightSprite()
{
    return Sprite::createWithSpriteFrameName(soundNameRight);
}

Sprite* CadenceMain::createStartSprite()
{
    return Sprite::createWithSpriteFrameName(start);
}

Sprite* CadenceMain::createStartButtonSprite()
{
    return Sprite::createWithSpriteFrameName(startButton);
}


}; // namespace

