#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <string>

class Message: public cocos2d::Layer
{
public:

    CREATE_FUNC( Message );

    void show( const std::string & p_str );

protected:
    virtual bool init( void );
    
    cocos2d::Label * m_messageLabel;
    
    cocos2d::ui::Scale9Sprite * m_messageBoxBG;

};


#endif //__MESSAGE_H__
