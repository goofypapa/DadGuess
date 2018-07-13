#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "cocos2d.h"
#include <string>

class Message: public cocos2d::Layer
{
public:
    static cocos2d::Layer * createMessage( void );

    CREATE_FUNC( Message );

    void show( const std::string & p_str );

    void hide( void );

protected:
    virtual bool init( void );

};


#endif //__MESSAGE_H__