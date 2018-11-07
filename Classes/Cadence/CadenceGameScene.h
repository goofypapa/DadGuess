//
//  CadenceGameScene.h
//  dadpat-mobile
//
//  Created by 吴思 on 2018/11/7.
//

#ifndef __CADENCE_GAME_SCENE_H__
#define __CADENCE_GAME_SCENE_H__

#include "BaseScene.h"

class CadenceGameScene : public BaseScene
{
public:
    CREATE_FUNC( CadenceGameScene );
    
protected:
    
    virtual bool init( void ) override;
    
};

#endif //__CADENCE_GAME_SCENE_H__
