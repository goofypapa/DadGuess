#ifndef __CADENCE_GAME_SCENE_H__
#define __CADENCE_GAME_SCENE_H__

#include "BaseScene.h"

class CadenceGameScene: public BaseScene
{
public:
    CREATE_FUNC( CadenceGameScene );

protected:
    virtual bool init( void ) override;
    virtual ~CadenceGameScene();
private:

};

#endif //__CADENCE_GAME_SCENE_H__