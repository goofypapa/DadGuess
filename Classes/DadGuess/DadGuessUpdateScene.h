//
//  DadGuessUpdateScene.h
//  dadpat-mobile
//
//  Created by 吴思 on 2018/10/29.
//

#ifndef __DAD_GUESS_UPDATE_SCENE_H__
#define __DAD_GUESS_UPDATE_SCENE_H__

#include "cocos2d.h"
#include "BaseScene.h"
#include "Http.h"
#include <list>
#include <functional>
#include <queue>
#include <map>
#include <mutex>

#include "DataTableFile.h"
#include "DataTableCard.h"
#include "DataTableCardBatch.h"
#include "DataTableCardAudio.h"

struct UpdateDownloadItem
{
public:
    std::string url;
    std::string md5;
    std::function< void( const DataFileInfo & ) > _downloadCallBack;
};

class DadGuessUpdateScene : public BaseScene
{
public:
    CREATE_FUNC( DadGuessUpdateScene );
    
    static void unCacheResource( void );
    
    static std::map< std::string, std::vector< std::pair< DataCardInfo, DataFileInfo > > > s_cardList;
    
protected:
    
    virtual bool init( void ) override;
    
    virtual void destroy( void );
private:
    
    void checkUpdateResponse( Http * p_http );
    void checkCardAudioUpdate( std::vector< DataCardAudioInfo > & p_oldCardAudioList, const std::vector< std::pair< std::string, std::string > > & p_seviceCardAudioList, const std::string & p_cardId, const DataCardAudioInfo::AudioType p_audioType );
    void downloadFile( void );
    
    static std::vector< DataCardBatchInfo > sm_cardBatchList;
    static std::vector< std::string > sm_loadImageList;

    bool m_needCehckUpdate;
    
    void checkUpdateDequeue( void );
    
    cocos2d::Label * m_messageLabel;
    cocos2d::Sprite * m_hand;
    
    int m_loadResourceCount;
    
    std::queue< std::function<void(void)> > m_checkUpdateQueue;
    std::function< void(void) > m_currUpdateFunc;
    std::list< Http * > m_checkUpdateHandlerList;
    
    std::map< Http *, std::string > m_checkCardUpdateBatchIdList;
    
    std::queue< UpdateDownloadItem > m_downloadList;
    std::map< std::string, DataCardBatchInfo > m_downloadCardBatchCoverList;
    std::map< std::string, DataCardAudioInfo > m_downloadAudioList;
    std::map< Http *, UpdateDownloadItem > m_downloadingList;
    
    static const char * sm_batchListApi, * sm_cardListApi, * sm_cardListApi2, * sm_cardTypeListApi;
    static const char * s_checkUpdateKey;

    static const long s_updateOverTime;
    
    std::mutex m_mutex;
};

#endif //__DAD_GUESS_UPDATE_SCENE_H__
