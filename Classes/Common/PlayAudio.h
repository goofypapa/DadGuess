//
//  PlayAudio.h
//  guess
//
//  Created by 吴思 on 2019/4/2.
//

#ifndef __PLAY_AUDIO_H__
#define __PLAY_AUDIO_H__

#include <string>
#include <functional>
#include <map>
#include <list>

class PlayAudio
{
public:
    
    typedef std::function<void (int, const std::string &)> FINISH_CALLBACK_FUNC;
    
    enum AudioState{
        ERROR  = -1,
        INITIALIZING,
        PLAYING,
        PAUSED
    };
    
    static int play( const std::string & p_audioPath );
    static void pause( const int p_playId );
    static void pauseAll();
    static void resume( const int p_playId );
    static void resumeAll();
    static void stop( const int p_playId );
    static void stopAll( const int p_playId );
    static AudioState getState( const int p_playId );
    
    static void setFinishCallback( int p_playId, const FINISH_CALLBACK_FUNC & p_finishCallbackFunc );
    
private:
    static int sm_playId;
};
#endif /* __PLAY_AUDIO_H__ */
