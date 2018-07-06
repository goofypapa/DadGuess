#ifndef __LOGIN_PHONE_LAYER_H__
#define __LOGIN_PHONE_LAYER_H__

#include "cocos2d.h"

USING_NS_CC;

class LoginPhoneLayer: public cocos2d::Layer
{
public: 
    CREATE_FUNC( LoginPhoneLayer );

    virtual bool init() override;
};


#endif //__LOGIN_PHONE_LAYER_H__
