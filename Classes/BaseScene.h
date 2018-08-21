#ifndef __BASE_SCENE_H__
#define __BASE_SCENE_H__

#include "cocos2d.h"
#include "DataTableFile.h"

class BaseScene : public cocos2d::Scene
{
public:

    static BaseScene * activityScene( void );

    virtual void refreshSource( const DataFile & p_dataInfo );

    virtual bool init( void ) override;
private:
    static BaseScene * sm_activityScene;
};

#endif //__BASE_SCENE_H__