#include "BaseScene.h"
#include "Common.h"


BaseScene * BaseScene::sm_activityScene = nullptr;

BaseScene * BaseScene::activityScene( void )
{
    return sm_activityScene;
}

void BaseScene::refreshSource( const DataFile & p_dataInfo )
{

}

bool BaseScene::init( void )
{
    if( !Scene::init() )
    {
        return false;
    }

    clearTouchAnswer();
    sm_activityScene = this;

    return true;
}