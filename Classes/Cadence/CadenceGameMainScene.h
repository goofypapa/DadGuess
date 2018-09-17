#ifndef __CADENCE_GAME_MAIN_SCENE_H__
#define __CADENCE_GAME_MAIN_SCENE_H__

#include "BaseScene.h"

class CadenceGameMainScene: public BaseScene
{
public:
    CREATE_FUNC( CadenceGameMainScene );

protected:
    virtual bool init( void ) override;
    virtual ~CadenceGameMainScene();
private:

};

#endif //__CADENCE_GAME_MAIN_SCENE_H__