#ifndef __PLAY_MANAGER_H__
#define __PLAY_MANAGER_H__

#include <map>
#include <functional>
#include <string>
#include <mutex>

class PlayManager
{

typedef std::function<void( int, std::string )> FinishCallBack;

public:
    static int Manage( const int p_playId, FinishCallBack p_finishCallBack = nullptr );
    static void StopAll( void );
private:
    static std::mutex sm_ManageStopMutex;
    static std::map< int, FinishCallBack > sm_finishCallBackList;
};
#endif //__PLAY_MANAGER_H__