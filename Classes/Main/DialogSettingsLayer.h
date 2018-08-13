//
//  DialogSettingsLayer.hpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/8/8.
//

#ifndef __DIALOG_SETTINGS_LAYER_H__
#define __DIALOG_SETTINGS_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <functional>

class DialogSettingsLayer: public cocos2d::Layer
{
public:
    
    void show();
    void hide();
    
    CREATE_FUNC( DialogSettingsLayer );
    
    std::function<void()> hideCallBack;
    
    void wifiConnect( cocos2d::Ref* pSender );
    
protected:
    void clearAllActions();
    virtual bool init( void ) override;
private:
    
    bool m_enableBGM;
    
    cocos2d::DrawNode * m_vague;
    cocos2d::Sprite * m_dialogCentBackground;
    cocos2d::ui::Button * m_dialogCentSaveButton;
    cocos2d::Vec2 m_dialogCentBackgroundHiedPos, m_dialogCentBackgroundShowPos, m_dialogCentSaveButtonHidePos, m_dialogCentSaveButtonShowPos;
    
};

#endif //__DIALOG_SETTINGS_LAYER_H__
