//
//  DadGuessCardListScene.h
//  dadpat-mobile
//
//  Created by 吴思 on 2018/11/21.
//

#ifndef __DAD_GUESS_CARD_LIST_SCENE_H__
#define __DAD_GUESS_CARD_LIST_SCENE_H__

#include "BaseScene.h"
#include <string>
#include <map>
#include <vector>
#include "DataTableCard.h"

class DadGuessCardListScene : public BaseScene
{
public:
    static DadGuessCardListScene * createScene( const std::string & p_groupId );
protected:
    virtual bool init( void ) override;
    virtual bool initWithGroupId( const std::string & p_groupId );
    
private:
    CREATE_FUNC( DadGuessCardListScene );
};

#endif //__DAD_GUESS_CARD_LIST_SCENE_H__
