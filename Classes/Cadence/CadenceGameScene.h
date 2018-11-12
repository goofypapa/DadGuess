//
//  CadenceGameScene.h
//  dadpat-mobile
//
//  Created by 吴思 on 2018/11/7.
//

#ifndef __CADENCE_GAME_SCENE_H__
#define __CADENCE_GAME_SCENE_H__

#include "BaseScene.h"
#include <vector>
#include <string>

#define PI 3.1415926535897932385f

class CadenceGameScene : public BaseScene
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
        bool tips;
        cocos2d::Sprite * ball;
    };
    
    enum Judge{
        Perfect = 0,
        Excellent,
        Leak
    };
    
    static CadenceGameScene * CreateScene( const std::string p_musicDir, const std::string p_musicName );
    
protected:
    
    CREATE_FUNC( CadenceGameScene );
    
    virtual bool init( void ) override;
    
    virtual bool initWithMusic( const std::string p_musicDir, const std::string p_musicName );
    
    virtual void update( float p_delta ) override;
    
    ~CadenceGameScene( void );
    
private:
    
    void startGame( void );
    void toneTouched( const int p_index );
    
    void changeJudge( const Judge p_judge );
    
    int m_perfectCount, m_excellentCount, m_leakCount;
    cocos2d::Label * m_perfectLabel, * m_excellentLabel, * m_leakLabel;
    
    cocos2d::Sprite * m_judgeSprite;
    cocos2d::Vec2 m_judgeSpritePos;
    
    int m_selectLevel, m_showBollIndex, m_backgroundMusicHandle;
    cocos2d::Vec2 m_startPointPos;
    float m_runwayLength, m_endLinePos, m_runwaySpace;
    
    bool m_playing, m_backgroundMusicCalibration;
    
    double m_startTime, m_musicTime, m_accumulativeTime;
    
    std::vector< cocos2d::Sprite * > m_tipsSpriteList;
    
    void loadMusicSource();
    std::string m_musicScoreDir, m_bgmPath;
    
    std::vector<Tone> m_musicScore;
    std::vector< GameTone > m_gameTones;
    
    static const char * sm_audioList[];
    cocos2d::Sprite * m_lightCenterp, * m_lightMidp, * m_lightOutp, * m_lightCentern, * m_lightMidn, * m_lightOutn;
};

#endif //__CADENCE_GAME_SCENE_H__
