#pragma once
#ifndef __MAIN_PERSONAL_LAYER_H__
#define __MAIN_PERSONAL_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <functional>

class MainDialogLayer: public cocos2d::Layer
{
public:

    void show();
    void hide();

    CREATE_FUNC( MainDialogLayer );

    std::function<void()> hideCallBack;

protected:
    void clearAllActions();
    virtual bool init( void ) override;
private:
    cocos2d::DrawNode * m_vague;
    cocos2d::Sprite * m_dialogCentBackground;
    cocos2d::ui::Button * m_dialogCentSaveButton;
    cocos2d::Vec2 m_dialogCentBackgroundHiedPos, m_dialogCentBackgroundShowPos, m_dialogCentSaveButtonHidePos, m_dialogCentSaveButtonShowPos;
};

#endif //__MAIN_PERSONAL_LAYER_H__
