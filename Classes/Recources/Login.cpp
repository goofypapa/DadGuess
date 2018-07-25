// ---------------------------------------
// Sprite definitions for Login
// Generated with TexturePacker 4.8.2
//
// https://www.codeandweb.com/texturepacker
// ---------------------------------------

#include "Login.hpp"

USING_NS_CC;

namespace TexturePacker
{

void Login::addSpriteFramesToCache()
{
    SpriteFrameCache *cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("Login.plist");
};

void Login::removeSpriteFramesFromCache()
{
    SpriteFrameCache *cache = SpriteFrameCache::getInstance();
    cache->removeSpriteFramesFromFile("Login.plist");
};


// ---------------------
// sprite name constants
// ---------------------
const std::string Login::loginBorder   = "LoginBorder.png";
const std::string Login::loginButton   = "LoginButton.png";
const std::string Login::loginEyeClose = "LoginEyeClose.png";
const std::string Login::loginEyeOpen  = "LoginEyeOpen.png";
const std::string Login::loginPhone    = "LoginPhone.png";
const std::string Login::loginSina     = "LoginSina.png";
const std::string Login::loginTitle    = "LoginTitle.png";
const std::string Login::loginWechat   = "LoginWechat.png";

// ---------------------------------------------------------
// convenience functions returing pointers to Sprite objects
// ---------------------------------------------------------
Sprite* Login::createLoginBorderSprite()
{
    return Sprite::createWithSpriteFrameName(loginBorder);
}

Sprite* Login::createLoginButtonSprite()
{
    return Sprite::createWithSpriteFrameName(loginButton);
}

Sprite* Login::createLoginEyeCloseSprite()
{
    return Sprite::createWithSpriteFrameName(loginEyeClose);
}

Sprite* Login::createLoginEyeOpenSprite()
{
    return Sprite::createWithSpriteFrameName(loginEyeOpen);
}

Sprite* Login::createLoginPhoneSprite()
{
    return Sprite::createWithSpriteFrameName(loginPhone);
}

Sprite* Login::createLoginSinaSprite()
{
    return Sprite::createWithSpriteFrameName(loginSina);
}

Sprite* Login::createLoginTitleSprite()
{
    return Sprite::createWithSpriteFrameName(loginTitle);
}

Sprite* Login::createLoginWechatSprite()
{
    return Sprite::createWithSpriteFrameName(loginWechat);
}


}; // namespace

