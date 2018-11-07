//
//  MainScene.hpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/7/24.
//

#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <functional>
#include "DialogSettingsLayer.h"
#include "DialogPersonalCenterLayer.h"
#include "DataTableUser.h"
#include "BaseScene.h"
#include <string>

class MainScene: public BaseScene
{
public:
    static cocos2d::Scene * CreateScene( void );
    CREATE_FUNC( MainScene );
    
    void personalHeadOnClick( void );
    void settingsOnClick( void );
    
    void icoTouchCallBack( int p_icoIndex );

    void updateUserInfo( void );

    virtual void refreshSource( const DataFile & p_fileInfo ) override;
    
protected:
    virtual bool init( void ) override;
    virtual ~MainScene();
private:
    cocos2d::Menu * m_mainMenu;
    DialogSettingsLayer * m_dialogSettings;
    DialogPersonalCenterLayer * m_dialogPersonalCenter;

    cocos2d::Sprite * m_personalHead;

    std::vector< cocos2d::ui::Button *> m_mainSceneButtons;
    std::function<void()> m_enableAllButton;
    std::function<void()> m_disenableAllButton;
    
    DataUser m_loginUser;
    
};

#endif /* __MAIN_SCENE_H__ */
