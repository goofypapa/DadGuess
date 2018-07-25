//
//  MainScene.hpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/7/24.
//

#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"

class MainScene: public cocos2d::Scene
{
public:
    static cocos2d::Scene * CreateScene( void );
    CREATE_FUNC( MainScene );
    
    void settingsCallBack( cocos2d::Ref* pSender );
    
protected:
    virtual bool init( void ) override;
    virtual ~MainScene();
private:
    
};

#endif /* __MAIN_SCENE_H__ */
