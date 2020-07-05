#include "PlayManager.h"
#include "PlayAudio.h"
#include "cocos2d.h"

USING_NS_CC;
using namespace experimental;


std::mutex PlayManager::sm_ManageStopMutex;
std::map< int, PlayManager::FinishCallBack > PlayManager::sm_finishCallBackList;

int PlayManager::Manage( const int p_playId, PlayManager::FinishCallBack p_finishCallBack )
{
    sm_finishCallBackList[p_playId] = p_finishCallBack;
    PlayAudio::setFinishCallback( p_playId, []( int p_playId, const std::string p_audio ){

        sm_ManageStopMutex.lock();
        auto t_it = sm_finishCallBackList.find( p_playId );
        if( t_it == sm_finishCallBackList.end() )
        {
            return;
        }

        if( t_it->second )
        {
            t_it->second( p_playId, p_audio );
        }

        sm_finishCallBackList.erase( t_it );
        sm_ManageStopMutex.unlock();
    } );

    return p_playId;
}

void PlayManager::StopAll( void )
{
    sm_ManageStopMutex.lock();
    for( auto t_it : sm_finishCallBackList )
    {
        PlayAudio::stop( t_it.first );
    }
    sm_finishCallBackList.clear();
    sm_ManageStopMutex.unlock();
}
