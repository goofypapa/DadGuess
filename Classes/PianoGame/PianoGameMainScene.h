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
#include "ui/CocosGUI.h"
#include "PianoGameMusicListLayer.h"

#include <vector>
#include <string>
#include <map>

#define PI 3.1415926535897932385f

class PianoGameMainScene : public BaseScene
{
public:
    
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
        std::string tone;
        cocos2d::Sprite * ball;
    };
    
    enum Judge{
        Perfect = 0,
        Excellent,
        Leak
    };
    
    static cocos2d::Scene * CreateScene( void );
    static std::string s_backgroundMusic;
    static std::string s_gameJson;
protected:
    CREATE_FUNC( PianoGameMainScene );
    virtual bool init( void ) override;
    virtual void update( float p_delta ) override;
    
    int contains( const cocos2d::Vec2 & p_point );
    void toneTouched( const int p_index );
    
    void toneTouchEnded( const int p_index );
    
    void changePlayButtonState();
    
    void changeJudge( const Judge p_judge );
    
    void loadMusic( void );
    
    virtual ~PianoGameMainScene( void );
    
    void buttonClick( const int p_tag );
private:
    float * m_segmentations;
    
    int m_backgroundMusicHandle;
    bool m_backgroundMusicCalibration;
    
    double m_startTime, m_accumulativeTime;
    
    bool m_isCountDown;
    
    cocos2d::Vec2 m_centerControlPosition;
    cocos2d::Vec2 m_startPos;
    float m_angleOffset, m_centerControlScale;
    
    double m_musicTime, m_currPlayTime, m_realTime, m_drumR, m_keyBottom;
    bool m_playing, m_backgroundMusicPlaying;
    
    int m_countDownNumber, m_showBollIndex;
    
    int m_perfectCount, m_excellentCount, m_leakCount;
    cocos2d::Label * m_perfectLabel, * m_excellentLabel, * m_leakLabel;
    
    cocos2d::Animation * m_toneTouchedUpAnimation, * m_toneTouchedDownAnimation;
    
    cocos2d::Sprite * m_countDownSprite, * m_leftRound, * m_rightRound, * m_judgeSprite;
    cocos2d::ui::Button * m_playState;
    
    std::vector< GameTone > m_gameTones;
    
    PianoGameMusicListLayer * m_musicListLayer;
    bool m_musicListShowState;
    
    std::map< int, int > m_touchIndex;
    std::vector< cocos2d::Sprite * > m_toneSpriteList;
    std::vector< cocos2d::Sprite * > m_toneUpAnimateSpriteList, m_toneDownAnimateSpriteList;
    std::vector< cocos2d::Sprite * > m_toneMarkSpriteList;
    std::vector<Tone> m_musicScore;
    std::vector<std::string> m_sequeueTone, m_sequeueToneNoOctave;
    std::vector< cocos2d::Vec2 > m_sequeueDirection;
    std::vector< cocos2d::ui::Button * > m_buttonList;
};

#endif //__PIANO_GAME_MAIN_SCENE_H__
