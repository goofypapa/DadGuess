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

class DadGuessMainScene : public BaseScene
{
public:
    CREATE_FUNC( DadGuessMainScene );
protected:
    virtual bool init( void ) override;
    
    static void scanCard( int p_rfid );
    
    static void playAudio( const DataCardAudioInfo & p_audioInfo );
    
    static BlueDeviceListener * sm_blueDeviceScanCardListener;
    
    virtual void destroy( void );
private:
    static bool sm_blueState;
    cocos2d::Sprite * m_blueConnectState;
    BlueDeviceListener * m_blueDeviceConnectedListener;
    
    static const char * sm_cardAudioInfoApi, * sm_cardAudioInfoApiAnimal;
    
};

#endif //__DAD_GUESS_MAIN_SCENE_H__
