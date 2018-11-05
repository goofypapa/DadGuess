//
//  PianoGameMusicListScrollView.h
//  dadpat-mobile
//
//  Created by 吴思 on 2018/10/31.
//

#ifndef __PIANO_GAME_MUSIC_LIST_SCROLL_VIEW_H__
#define __PIANO_GAME_MUSIC_LIST_SCROLL_VIEW_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <map>

class PianoGameMusicListScrollView : public cocos2d::ui::ScrollView
{
public:
    static std::vector< std::pair< std::string, std::string > > sm_musicList;
    static PianoGameMusicListScrollView * create( const cocos2d::Size & p_contentSize, const float p_itemHeight );
    
    int selectIndex;
protected:
    CREATE_FUNC( PianoGameMusicListScrollView );
    virtual bool init( void ) override;
    
    virtual bool initWithContentSize( const cocos2d::Size & p_contentSize, const float p_itemHeight );
    
    virtual bool onTouchBegan( cocos2d::Touch * p_touch, cocos2d::Event * p_unusedEvent) override;
    virtual void onTouchMoved( cocos2d::Touch * p_touch, cocos2d::Event * p_unusedEvent) override;
    virtual void onTouchEnded( cocos2d::Touch * p_touch, cocos2d::Event * p_unusedEvent) override;
    virtual void onTouchCancelled( cocos2d::Touch * p_touch, cocos2d::Event * p_unusedEvent) override;
private:
    float m_itemHeight;
    cocos2d::Sprite * m_selectedSprite;
    std::map< int, cocos2d::Vec2 > m_touchBeginLocationList;
};

#endif //__PIANO_GAME_MUSIC_LIST_SCROLL_VIEW_H__
