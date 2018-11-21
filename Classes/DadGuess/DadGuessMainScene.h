//
//  DadGuessMainScene.h
//  dadpat-mobile
//
//  Created by 吴思 on 2018/11/20.
//

#ifndef __DAD_GUESS_MAIN_SCENE_H__
#define __DAD_GUESS_MAIN_SCENE_H__

#include "BaseScene.h"

class DadGuessMainScene : public BaseScene
{
public:
    CREATE_FUNC( DadGuessMainScene );
protected:
    virtual bool init( void ) override;
    
};

#endif //__DAD_GUESS_MAIN_SCENE_H__
