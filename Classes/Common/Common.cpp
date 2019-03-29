//
//  Common.cpp
//  DadGuess-mobile
//
//  Created by 吴思 on 2018/7/6.
//

#include "cocos2d.h"
#include "Common.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

std::map< cocos2d::Ref *, ButtonTouchEvents > s_ButtonEventCache;

float adaptation( Vec2 p_sourcePixel )
{
    
    static Vec2 t_phonePixel = Director::getInstance()->getOpenGLView()->getFrameSize();
    static float t_result = MIN( t_phonePixel.x / p_sourcePixel.x , t_phonePixel.y / p_sourcePixel.y );
    
    return t_result;
}

void touchAnswer( Button * p_button, std::function<void( cocos2d::Ref * )> p_onTouchEnded, float p_scaleTo, float p_sacleOriginal )
{

    if( s_ButtonEventCache.find( p_button ) != s_ButtonEventCache.end() )
    {
        s_ButtonEventCache[p_button].onTouchEnded = p_onTouchEnded;
        return;
    }

    ButtonTouchEvents t_buttonTouchEvents;
    t_buttonTouchEvents.onTouchEnded = p_onTouchEnded;
    s_ButtonEventCache[p_button] = t_buttonTouchEvents;

    p_button->addTouchEventListener( [p_scaleTo, p_sacleOriginal]( Ref* p_ref, Widget::TouchEventType p_touchType ){

        ButtonTouchEvents t_buttonTouchEvents = s_ButtonEventCache[p_ref];

        Button * t_button = (Button *)p_ref;

        switch( p_touchType )
        {
            case Widget::TouchEventType::BEGAN:
                t_button->stopAllActions();
                t_button->runAction( ScaleTo::create( 0.1f, p_scaleTo ) );
            break;
            case Widget::TouchEventType::CANCELED:
                t_button->stopAllActions();
                t_button->runAction( ScaleTo::create( 0.05f, p_sacleOriginal ) );
            break;
            case Widget::TouchEventType::ENDED:
                t_button->stopAllActions();
                t_button->runAction( ScaleTo::create( 0.05f, p_sacleOriginal ) );
                
                t_button->runAction( Sequence::create( ScaleTo::create( 0.05f, p_sacleOriginal ), CallFunc::create( [t_buttonTouchEvents, p_ref](){
                    t_buttonTouchEvents.onTouchEnded( p_ref );
                } ), nullptr ) );
            break;
            case Widget::TouchEventType::MOVED:
                
            break;
        };

    } );
}

void touchAnswer( cocos2d::ui::Button * p_button, std::function<void( cocos2d::Ref * )> p_onTouchBegin, std::function<void( cocos2d::Ref * )> p_onTouchCanceled, std::function<void( cocos2d::Ref * )> p_onTouchEnded, std::function<void( cocos2d::Ref * )> p_onTouchMoved )
{
    if( s_ButtonEventCache.find( p_button ) != s_ButtonEventCache.end() )
    {
        s_ButtonEventCache[p_button].onTouchBegin = p_onTouchBegin;
        s_ButtonEventCache[p_button].onTouchCanceled = p_onTouchCanceled;
        s_ButtonEventCache[p_button].onTouchEnded = p_onTouchEnded;
        s_ButtonEventCache[p_button].onTouchMoved = p_onTouchMoved;
        return;
    }
    
    ButtonTouchEvents t_buttonTouchEvents;
    t_buttonTouchEvents.onTouchBegin = p_onTouchBegin;
    t_buttonTouchEvents.onTouchCanceled = p_onTouchCanceled;
    t_buttonTouchEvents.onTouchEnded = p_onTouchEnded;
    t_buttonTouchEvents.onTouchMoved = p_onTouchMoved;
    s_ButtonEventCache[p_button] = t_buttonTouchEvents;
    
    p_button->addTouchEventListener( []( Ref* p_ref, Widget::TouchEventType p_touchType ){
        
        ButtonTouchEvents t_buttonTouchEvents = s_ButtonEventCache[p_ref];

        switch( p_touchType )
        {
            case Widget::TouchEventType::BEGAN:
                if( t_buttonTouchEvents.onTouchBegin != nullptr )
                {
                    t_buttonTouchEvents.onTouchBegin( p_ref );
                }
                break;
            case Widget::TouchEventType::CANCELED:
                if( t_buttonTouchEvents.onTouchCanceled != nullptr )
                {
                    t_buttonTouchEvents.onTouchCanceled( p_ref );
                }
                break;
            case Widget::TouchEventType::ENDED:
                if( t_buttonTouchEvents.onTouchEnded != nullptr )
                {
                    t_buttonTouchEvents.onTouchEnded( p_ref );
                }
                break;
            case Widget::TouchEventType::MOVED:
                if( t_buttonTouchEvents.onTouchMoved != nullptr )
                {
                    t_buttonTouchEvents.onTouchMoved( p_ref );
                }
                break;
        };
        
    } );
}

void clearTouchAnswer( void )
{
    s_ButtonEventCache.clear();
}

std::vector<std::string> split( const std::string & p_str, const std::string & p_clapboard )
{
    std::vector<std::string> t_result;
    std::string::size_type t_posStart, t_posEnd;

    t_posStart = 0;
    while(1)
    {
        t_posEnd = p_str.find( p_clapboard, t_posStart );
        
        if( std::string::npos == t_posEnd )
        {
            t_result.push_back( p_str.substr( t_posStart ) );
            break;
        }

        t_result.push_back( p_str.substr( t_posStart, t_posEnd - t_posStart ) );
        t_posStart = t_posEnd + p_clapboard.size();
    }

    return t_result;
}

std::vector<std::string> split( const std::string & p_str, const char p_clapboard )
{
    return split( p_str, std::string( 1, p_clapboard ) );
}
static const std::string s_doubleQuotationMarkSplaceholder = ">^DoubleQuotationMarkSplaceholder$<";
std::string strToSqlStr( const std::string & p_str )
{
    std::stringstream t_sstr;
    const std::string t_replaceStr = "\"";
    
    auto t_items = split( p_str, t_replaceStr );

    t_sstr << t_items[0];
    for( int i = 1; i < t_items.size(); ++i )
    {
        t_sstr << s_doubleQuotationMarkSplaceholder << t_items[i];
    }

    return t_sstr.str();
}
std::string sqlStrToStr( const std::string & p_sqlStr )
{
    std::stringstream t_sstr;
    const std::string t_replaceStr = "\"";
    
    auto t_items = split( p_sqlStr, s_doubleQuotationMarkSplaceholder );

    t_sstr << t_items[0];
    for( int i = 1; i < t_items.size(); ++i )
    {
        t_sstr << t_replaceStr << t_items[i];
    }

    return t_sstr.str();
}

std::string toString( __Dictionary & p_dirtionary )
{
    std::stringstream t_result;
    
    t_result << "{";
    
    try {
        __Array *allKeys = p_dirtionary.allKeys();
        allKeys->retain();
        for (int i = 0; i < allKeys -> count(); i++)
        {
            __String *key = (__String *)allKeys -> getObjectAtIndex(i);
            Ref * obj = p_dirtionary.objectForKey(key -> getCString());

            t_result << "\"" << key->getCString() << "\"" << ":" << toString( obj );
            
            if( i < allKeys->count() - 1 )
            {
                t_result << ",";
            }
        }
        allKeys->release();
    } catch (...) {
        printf( " __Dictionary to string error\n" );
    }
    
    t_result << "}";
    return t_result.str();
}

std::string toString( Ref * p_obj )
{
    std::stringstream t_result;
    
    if (dynamic_cast<__String *>( p_obj ))
    {

        std::string t_str = dynamic_cast<__String *>( p_obj ) -> getCString();

        size_t t_it = 0;
        while( ( t_it = t_str.find("\"", t_it ) ) != std::string::npos )
        {
            t_str = t_str.replace( t_it, 1, "\\\"" );
            t_it += 2;
        }

        t_result << "\"" << t_str << "\"";
    }
    else if (dynamic_cast<__Integer *>( p_obj ))
    {
        t_result << dynamic_cast<__Integer *>( p_obj ) -> getValue();
    }
    else if (dynamic_cast<__Double *>( p_obj ))
    {
        t_result << dynamic_cast<__Double *>( p_obj ) -> getValue();
    }
    else if (dynamic_cast<__Bool *>( p_obj ))
    {
        t_result << ( dynamic_cast<__Bool *>( p_obj ) -> getValue() ? "true" : "false" );
    }
    else if (dynamic_cast<__Float *>( p_obj ))
    {
        t_result << dynamic_cast<__Float *>( p_obj ) -> getValue();
    }
    else if (dynamic_cast<__Array *>( p_obj ))
    {
        
        __Array * t_array = dynamic_cast<__Array *>( p_obj );
        
        t_result << "[";
        
        for( int i = 0 ; i < t_array->count(); ++i )
        {
            t_result << toString( t_array->getObjectAtIndex( i ) );
            if( i < t_array->count() - 1 )
            {
                t_result << ",";
            }
        }
        
        t_result << "]";
    }
    else if (dynamic_cast<__Dictionary *>( p_obj ))
    {
        t_result << toString( *dynamic_cast<__Dictionary *>( p_obj ) );
    }else{
        t_result << "null";
    }
    
    return t_result.str();
}


const std::string & writeFilePath( void )
{
    static std::string sm_writeFilePath = "";
    if( sm_writeFilePath.empty() )
    {
        sm_writeFilePath = cocos2d::FileUtils::getInstance()->getWritablePath();
    }

    return sm_writeFilePath;
}

std::string fullFilePath( const std::string & p_file )
{
    return writeFilePath() + p_file;
}

std::string fileSuffix( const std::string p_fileName )
{
    auto t_list  = split( p_fileName, "." );
    if( t_list.size() > 1 ){
        return t_list[t_list.size() - 1];
    }

    return "";
}

cocos2d::Color4F createColor4FWithStr( const std::string & p_colorStr, const float p_alpha )
{
    float r, g, b, a;
    r = g = b = 0.0f;
    a = p_alpha;
    
    std::string t_colorStr = p_colorStr;
    
    auto t_it = t_colorStr.find( "#" );
    if( t_it != std::string::npos )
    {
        t_colorStr.replace( t_it, 1, "" );
    }
    
    if( t_colorStr.length() >= 6 )
    {
        int t_r = std::stoi( t_colorStr.substr( 0, 2 ), nullptr, 16 );
        int t_g = std::stoi( t_colorStr.substr( 2, 2 ), nullptr, 16 );
        int t_b = std::stoi( t_colorStr.substr( 4, 2 ), nullptr, 16 );
        
        if( t_r )
        {
            r = t_r / 255.0f;
        }
        if( t_g )
        {
            g = t_g / 255.0f;
        }
        if( t_b )
        {
            b = t_b / 255.0f;
        }
    }
    return cocos2d::Color4F( r, g, b, a );
}

