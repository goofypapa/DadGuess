//
//  CadenceGameMusicListScrollView.h
//  dadpat-mobile
//
//  Created by 吴思 on 2018/11/8.
//

#ifndef __CADENCE_GAME_MUSIC_LIST_SCROLL_VIEW_H__
#define __CADENCE_GAME_MUSIC_LIST_SCROLL_VIEW_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <vector>
#include <map>
#include <functional>

class CadenceGameMusicListScrollView : public cocos2d::ui::ScrollView
{
public:
    CREATE_FUNC( CadenceGameMusicListScrollView );
    
    virtual bool initWithData( const std::vector< std::pair< std::string, std::string > > & P_dataList );
    
    std::function<void( const int p_index )> onTouched;
protected:
    
    virtual bool init( void ) override;
    
    virtual bool onTouchBegan( cocos2d::Touch * p_touch, cocos2d::Event * p_unusedEvent) override;
    virtual void onTouchMoved( cocos2d::Touch * p_touch, cocos2d::Event * p_unusedEvent) override;
    virtual void onTouchEnded( cocos2d::Touch * p_touch, cocos2d::Event * p_unusedEvent) override;
    
private:
    std::map< int, cocos2d::Vec2 > m_touchBeginLocationList;
};

#endif //__CADENCE_GAME_MUSIC_LIST_SCROLL_VIEW_H__
