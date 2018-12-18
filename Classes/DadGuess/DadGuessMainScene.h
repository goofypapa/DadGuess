//
//  DadGuessMainScene.h
//  dadpat-mobile
//
//  Created by 吴思 on 2018/11/20.
//

#ifndef __DAD_GUESS_MAIN_SCENE_H__
#define __DAD_GUESS_MAIN_SCENE_H__

#include "BaseScene.h"
#include "BlueDeviceListener.h"
#include "cocos2d.h"
#include <map>
#include <vector>
#include "DataTableCardAudio.h"
#include "DataTableUser.h"
#include "DialogPersonalCenterLayer.h"

#include "ui/CocosGUI.h"

class DadGuessMainScene : public BaseScene
{
public:
    CREATE_FUNC( DadGuessMainScene );
    void updateUserInfo( void );
    static bool sm_blueState;
protected:
    virtual bool init( void ) override;
    
    static void scanCard( int p_rfid );
    
    static void playAudio( const DataCardAudioInfo & p_audioInfo );
    
    static BlueDeviceListener * sm_blueDeviceScanCardListener;
    
    virtual void refreshSource( const DataFileInfo & p_fileInfo ) override;
    
    virtual void destroy( void );
private:
    cocos2d::ui::Button * m_blueConnectState;
    BlueDeviceListener * m_blueDeviceConnectedListener;
    cocos2d::Sprite * m_personalHead;
    DataUserInfo m_loginUser;
    DialogPersonalCenterLayer * m_dialogPersonalCenter;
    
    bool m_enableMainButton;
    
    static const char * sm_cardAudioInfoApi, * sm_cardAudioInfoApiAnimal;
    static const char * sm_checkoutCardAudioUpdateKey;
    static const long sm_checkoutCardAudioUpdateOverTime;
};

#endif //__DAD_GUESS_MAIN_SCENE_H__
