//
//  PinaoGameMainScene.h
//  dadpat-mobile
//
//  Created by 吴思 on 2018/10/24.
//

#ifndef __PIANO_GAME_MAIN_SCENE_H__
#define __PIANO_GAME_MAIN_SCENE_H__

#include "cocos2d.h"
#include "BaseScene.h"

#include <vector>
#include <string>

#define PI 3.1415926535897932385f

struct Tone
{
public:
    double time;
    std::vector<std::string> tone;
};

struct GameTone
{
public:
    double time;
    int index;
    cocos2d::Sprite * ball;
};

class PianoGameMainScene : public BaseScene
{
public:
    static cocos2d::Scene * CreateScene( void );
    static std::string s_backgroundMusic;
    static std::string s_gameJson;
protected:
    CREATE_FUNC( PianoGameMainScene );
    virtual bool init( void ) override;
    virtual void update( float p_delta ) override;
    
    virtual ~PianoGameMainScene( void );
private:
    float * m_segmentations;
    
    cocos2d::Vec2 m_centerControlPosition;
    cocos2d::Vec2 m_startPos;
    float m_angleOffset, m_centerControlScale;
    
    double m_musicTime, m_currPlayTime;
    bool m_playing, m_backgroundMusicPlaying;
    
    int m_countDownNumber, m_showBollIndex, m_touchToneIndex;
    cocos2d::Label * m_countDownLabel;
    
    std::vector< GameTone > m_gameTones;
    
    std::vector<Tone> m_musicScore;
    std::vector<std::string> m_sequeueTone;
    std::vector< cocos2d::Vec2 > m_sequeueDirection;
};

#endif //__PIANO_GAME_MAIN_SCENE_H__
