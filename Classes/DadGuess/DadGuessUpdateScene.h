//
//  DadGuessUpdateScene.h
//  dadpat-mobile
//
//  Created by 吴思 on 2018/10/29.
//

#ifndef __DAD_GUESS_UPDATE_SCENE_H__
#define __DAD_GUESS_UPDATE_SCENE_H__

#include "cocos2d.h"
#include "BaseScene.h"
//#include "Data"

#define PI 3.1415926535897932385f

class DadGuessUpdateScene : public BaseScene
{
public:
    static cocos2d::Scene * CreateScene( void );
    
protected:
    CREATE_FUNC( DadGuessUpdateScene );
    
    virtual bool init( void ) override;
    
    virtual ~DadGuessUpdateScene( void );
private:
    static const char * sm_batchListApi;
};

#endif //__DAD_GUESS_UPDATE_SCENE_H__
