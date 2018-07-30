#pragma once
#ifndef __MAIN_PERSONAL_LAYER_H__
#define __MAIN_PERSONAL_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class MainPersonalLayer: public cocos2d::Layer
{
public:

    void show();
    void hide();

    CREATE_FUNC( MainPersonalLayer );

protected:

    virtual bool init( void ) override;
private:
    cocos2d::DrawNode * m_vague;
    cocos2d::Sprite * m_personalCentBackground;
    cocos2d::ui::Button * m_personalContSaveButton;
    cocos2d::Vec2 m_personalCentBackgroundHiedPos, m_personalCentBackgroundShowPos, m_personalContSaveButtonHidePos, m_personalContSaveButtonShowPos;
};

#endif //__MAIN_PERSONAL_LAYER_H__