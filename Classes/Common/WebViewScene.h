//
//  WebViewScene.hpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/8/2.
//

#ifndef __WEB_VIEW_SCENE_H__
#define __WEB_VIEW_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <string>
#include <map>
#include <functional>
#include "DataTableFile.h"
#include "BaseScene.h"
#include "Http.h"

struct Ajax
{
    std::string url;
    std::string key;
    std::string successCallBack;
    std::string fialCallBack;
};

class WebViewScene : public BaseScene
{
public:
    static cocos2d::Scene * createWithUrl( const std::string & p_url, const bool p_orientation );
protected:
    CREATE_FUNC( WebViewScene );
    virtual bool init( void ) override;
    virtual bool initWithUrl( const std::string & p_url, const bool p_orientation );
    
    virtual void refreshSource( const DataFileInfo & p_dataInfo ) override;
    
    void loadAudio( const std::string & p_audioUrl, std::function<void( DataFileInfo p_audioFile )> p_loadAudioCallBack );
    void deleteAudio( const std::string & p_audioUrl );
    
    void playAudio( const std::string & p_audioUrl, const std::string & p_finishCallBack );
    void stopAudio( const std::string & p_audioUrl );
    
    void stopAllAudio( void );
    
    virtual ~WebViewScene( void );
private:
    cocos2d::experimental::ui::WebView * m_webview;
    std::string m_url;
    bool m_webOrientation;
    bool m_firstLoad;
    
    
    std::map< Http *, Ajax > s_ajaxPool;
    
    std::map< std::string, std::function<void( DataFileInfo p_audioFile )> > s_downloadList;
    
    std::map< std::string, unsigned int > s_playList;
    std::map< std::string, std::string > m_playCallBackList;
    
    cocos2d::Sprite * m_action;
    
    std::string urlRepair( std::string p_url );
};

#endif //__WEB_VIEW_SCENE_H__
