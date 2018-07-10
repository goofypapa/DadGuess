//
//  Common.cpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/7/6.
//

#include "Common.h"


float adaptation( Vec2 p_sourcePixel )
{
    
    static Vec2 t_phonePixel = Director::getInstance()->getOpenGLView()->getFrameSize();
    
    return MIN( t_phonePixel.x / p_sourcePixel.x , t_phonePixel.y / p_sourcePixel.y );
}
