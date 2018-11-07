//
//  CadenceGameLoaderScene.h
//  dadpat-mobile
//
//  Created by 吴思 on 2018/11/6.
//

#ifndef __CADENCE_GAME_LOADER_SCENE_H__
#define __CADENCE_GAME_LOADER_SCENE_H__

#include "BaseScene.h"

class CadenceGameLoaderScene : public BaseScene
{
public:
    CREATE_FUNC( CadenceGameLoaderScene );
    
    static void uncacheResource( void );
    
protected:
    virtual bool init( void ) override;
    
    void cacheResource( void );
    
    virtual ~CadenceGameLoaderScene();
private:
    size_t m_loadResourceCount;
    static const char * sm_loadImgList[];
    static const char * sm_loadAudioList[];
};

#endif //__CADENCE_GAME_LOADER_SCENE_H__
