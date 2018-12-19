#ifndef __DAD_GUESS_BLUE_SCENE_H__
#define __DAD_GUESS_BLUE_SCENE_H__

#include "BaseScene.h"
#include "BlueDeviceListener.h"
#include <vector>
#include <string>


class DadGuessBlueScene : public BaseScene
{
public:
    CREATE_FUNC( DadGuessBlueScene );
protected:
    virtual bool init( void ) override;

    virtual ~DadGuessBlueScene( void );
private:
    BlueDeviceListener * m_blueDeviceListener;

    std::vector< std::string > m_blueDeviceList;
};

#endif //__DAD_GUESS_BLUE_SCENE_H__