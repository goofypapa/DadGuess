#ifndef __DAD_GUESS_BLUE_SCENE_H__
#define __DAD_GUESS_BLUE_SCENE_H__

#include "BaseScene.h"

class DadGuessBlueScene : public BaseScene
{
public:
    CREATE_FUNC( DadGuessBlueScene );
protected:
    virtual bool init( void ) override;

};

#endif //__DAD_GUESS_BLUE_SCENE_H__