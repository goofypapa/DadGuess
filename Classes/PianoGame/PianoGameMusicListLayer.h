//
//  PianoGameMusicListLayer.h
//  dadpat-mobile
//
//  Created by 吴思 on 2018/10/30.
//

#ifndef __PIANO_GAME_MUSIC_LIST_LAYER_H__
#define __PIANO_GAME_MUSIC_LIST_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "PianoGameMusicListScrollView.h"
#include <list>
#include <vector>

class PianoGameMusicListLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC( PianoGameMusicListLayer );
    
    void show();
    void hide();
    
    std::pair< std::string, std::string > getSelectMusic( void );
    
protected:
    virtual bool init( void ) override;
private:
    
    std::vector< std::pair< std::string, std::string > > m_musicList;
    
    cocos2d::Vec2 m_contentPos, m_buttonFirstPos;
    std::list< cocos2d::ui::Button * > m_buttonList;
    cocos2d::ui::Scale9Sprite * m_contentView;
    PianoGameMusicListScrollView * m_musicListScrollView;
};

#endif //__PIANO_GAME_MUSIC_LIST_LAYER_H__
