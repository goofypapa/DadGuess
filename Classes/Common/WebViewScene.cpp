//
//  WebViewScene.cpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/8/2.
//

#include "WebViewScene.h"
#include "Common.h"
#include "MainScene.h"
#include <sstream>
#include "Http.h"
#include "AudioEngine.h"

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

#include "DataTableCardBatch.h"

#include "DadGuessCardListScene.h"

USING_NS_CC;
using namespace  rapidjson;
using namespace cocos2d::ui;
using namespace experimental;


WebViewScene * WebViewScene::sm_instance = nullptr;

cocos2d::Scene * WebViewScene::createWithDir( const std::string & p_dir, const bool p_orientation, const std::string & p_resourceId )
{
    auto t_res = create();
    
    if( !t_res->initWithDir( p_dir, p_orientation, p_resourceId ) ){
        delete t_res;
        t_res = nullptr;
    }
    
    return t_res;
}

bool WebViewScene::init( void )
{
    m_firstLoad = true;
    sm_instance = this;
    return true;
}


bool WebViewScene::initWithDir( const std::string & p_dir, const bool p_orientation, const std::string & p_resourceId )
{
    m_webOrientation = p_orientation;
    m_dir = p_dir;
    m_resourceId = p_resourceId;
    m_groupId = "";
    
    static const char * s_webDirList[] = { "Web_Animal", "Web_ABC", "Web_Astronomy", "Web_Earth", "Web_WorldHistory", "Web_HistoryChronology" };
    if( !p_resourceId.empty() )
    {
        m_groupId = m_dir;
        int t_index = (int)( std::find( DataCardBatchInfo::s_batchIdList.begin(), DataCardBatchInfo::s_batchIdList.end(), m_groupId ) - DataCardBatchInfo::s_batchIdList.begin() );
        m_dir = s_webDirList[t_index];
    }
    
    if( !BaseScene::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    

    printf( "visibleSize.width: %f, visibleSize.height: %f \n", visibleSize.width, visibleSize.height );
    printf( "origin.x: %f, origin.y: %f \n", origin.x, origin.y );

    auto contentSize = getContentSize();
    printf( "contentSize.width: %f, contentSize.height: %f \n", contentSize.width, contentSize.height );
    
    std::stringstream t_splistName;
    
    t_splistName << "Web/" << m_dir << ".plist";
    
    m_action = Sprite::create();
    m_action->setScale( adaptation() );
    m_action->setPosition( Vec2( origin.x + visibleSize.width * 0.5f, origin.y + visibleSize.height * 0.5f ) );
    addChild( m_action );
    
    if( FileUtils::getInstance()->isFileExist( t_splistName.str() ) )
    {
        
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile( t_splistName.str() );
        Animation * t_animation = Animation::create();
        for( int i = 0; i < 25; ++i )
        {
            std::stringstream t_spriteName;
            t_spriteName << "Frame_" << ( i + 1 ) << ".png";
            auto t_frame = SpriteFrameCache::getInstance()->getSpriteFrameByName( t_spriteName.str() );
            t_animation->addSpriteFrame( t_frame );
        }
        
        t_animation->setDelayPerUnit(0.04f);
        t_animation->setLoops( -1 );
        
        Animate * t_animate = Animate::create( t_animation );

        m_action->runAction( t_animate );
    }
    
    
    m_webview = experimental::ui::WebView::create();

    m_webview->setContentSize( Size( visibleSize.width, visibleSize.height ) );

    m_webview->setPosition( Vec2( origin.x + visibleSize.width * 0.5f, origin.y + visibleSize.height * 0.5f ) );
//    m_webview->loadURL( p_url );

    std::stringstream t_surl, t_parameter;
    if( m_resourceId.empty() )
    {
        t_surl << "Web/" << m_dir << "/index.html";
    }else{
        int t_index = (int)( std::find( DataCardBatchInfo::s_batchIdList.begin(), DataCardBatchInfo::s_batchIdList.end(), p_dir ) - DataCardBatchInfo::s_batchIdList.begin() );
        
        t_surl << "Web/" << m_dir;
        switch (t_index) {
            case 1:
                t_surl << "/ting.html";
                break;
            case 3:
                t_surl << "/index.html";
                t_parameter << "#/details";
                break;
            case 4:
                t_surl << "/lunboNew.html";
                break;
            default:
                t_surl << "/details.html";
                break;
        }
        t_parameter << "?resourceId=" << m_resourceId;
    }
#ifdef TARGET_OS_IPHONE
    auto fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename( t_surl.str() );
#else
    auto fullPath = t_surl.str();
#endif
    
    t_surl.str( "" );
#ifdef TARGET_OS_IPHONE
    t_surl << fullPath << t_parameter.str();
#else
    t_surl << "file:///android_asset/" << fullPath << t_parameter.str();
#endif

    m_webview->loadURL( t_surl.str() );
    
    m_webview->setJavascriptInterfaceScheme( "goofypapa" );
    m_webview->setBounces(false);
    
//    m_webview->setOnEnterCallback( [this](){
//        printf( "---------> setOnEnterCallback" );
//
//    } );

    m_webview->setOnDidFinishLoading([this]( experimental::ui::WebView * p_scene, std::string p_url ){

        printf("---------------> setOnDidFinishLoading");
        
        std::stringstream t_sstrJsCode;
        t_sstrJsCode << "window.goofypapaGame = true;"  << "window.goofypapaToken=function(){return \"" << Http::token << "\";};if(window.goofypapaInit){ window.goofypapaInit();};";
        
        m_webview->evaluateJS( t_sstrJsCode.str() );
        
#ifdef TARGET_OS_IPHONE
        m_webview->evaluateJS( "document.documentElement.style.webkitTouchCallout = \"none\";document.documentElement.style.webkitUserSelect = \"none\";" );
#endif

        if( m_firstLoad ){
            m_webview->setOpacityWebView( 0.0f );
            m_webview->runAction( ActionFloat::create( 0.2f, 0.0f, 1.0f, [this]( float p_val){
                m_webview->setOpacityWebView( p_val );
            }) );
            m_firstLoad = false;
        }
        
        if( m_action && m_action->isRunning() )
        {
            m_action->stopAllActions();
            m_action->removeFromParent();
            m_action = nullptr;
        }

    //    m_webview->evaluateJS( "alert(window.goofypapaGame);" );
    //    m_webview->evaluateJS( "alert(window.goofypapaToken);" );
    });

    m_webview->setOnJSCallback( [this]( experimental::ui::WebView * p_scene, std::string p_url ){
        
        printf( "---> %s \n", p_url.c_str() );
        
        std::string t_key = "goofypapa://";
        
        if( p_url.find( t_key ) != 0 )
        {
            return;
        }
        
        auto t_url = p_url.replace( 0, t_key.size(), "" );
        
        auto t_list = split( t_url, ";" );
        
        for( auto t_item : t_list )
        {
            if( t_item.compare( "back" ) == 0 )
            {
                if( m_webOrientation ){
                    setAppOrientation( false );
                }
                stopAllAudio();
                if( m_resourceId.empty() )
                {
                    Director::getInstance()->replaceScene( MainScene::create() );
                }else{
                    Director::getInstance()->replaceScene( DadGuessCardListScene::createScene( m_groupId ) );
                }
            }
            
            if( t_item.compare( "stopAllAudio" ) == 0 )
            {
                stopAllAudio();
            }
            
            auto t_funcList = split( t_item, "," );
            
            if( t_funcList[0].compare( "loadAudio" ) == 0 )
            {
                if( t_funcList.size() != 2 )
                {
                    return;
                }
                
                loadAudio( urlRepair( t_funcList[1] ), nullptr);
            }
            
            if( t_funcList[0].compare( "playAudio" ) == 0 )
            {
                if( t_funcList.size() == 2 )
                {
                    playAudio( urlRepair( t_funcList[1] ), "" );
                } else if( t_funcList.size() == 3 )
                {
                    playAudio( urlRepair( t_funcList[1] ), t_funcList[2] );
                }
                
                return;
                
            }
            
            if( t_funcList[0].compare( "stopAudio" ) == 0 )
            {
                if( t_funcList.size() != 2 )
                {
                    return;
                }
                stopAudio( urlRepair( t_funcList[1] ) );
            }
            
            if( t_funcList[0].compare( "deleteAudio" ) == 0 )
            {
                if( t_funcList.size() != 2 )
                {
                    return;
                }
                deleteAudio( urlRepair( t_funcList[1] ) );
            }
            
            if( t_funcList[0].compare( "Post" ) == 0 || t_funcList[0].compare( "Get" ) == 0 )
            {
                if( t_funcList.size() < 4 )
                {
                    return;
                }
                
                Ajax t_ajax;
                
                t_ajax.url = urlRepair( t_funcList[1] );
                t_ajax.key = "";
                t_ajax.successCallBack = t_funcList[2];
                t_ajax.fialCallBack = t_funcList[3];
                
                std::stringstream t_skey;
                
                t_skey << t_funcList[0] << t_ajax.url;
                
                std::map< std::string, std::string > t_parameter;
                
                for( int i = 4; i < t_funcList.size(); ++i )
                {
                    auto t_p = split( t_funcList[i], ":");
                    if( t_p.size() != 2 )
                    {
                        continue;
                    }
                    t_skey << "," << t_p[0] << ":" << t_p[1];
                    t_parameter[t_p[0]] = t_p[1];
                }
                
                t_ajax.key = t_skey.str();
                
                auto t_successCallBack = [this]( Http * p_http, std::string p_res ){
                    
                    if( s_ajaxPool.find( p_http ) != s_ajaxPool.end() )
                    {
                        auto & t_ajax = s_ajaxPool[ p_http ];
                        std::stringstream t_sstr;
                        t_sstr << t_ajax.successCallBack << "(\"" << t_ajax.key << "\", " << p_res << ");";
                        std::string t_str = t_sstr.str();
                        m_httpCallBackList.push( [this, t_str](){
                            printf( "----------Post: %s \n", t_str.c_str() );
                            m_webview->evaluateJS( t_str );
                        });
                        
                        s_ajaxPool.erase( p_http );
                    }
                };
                
                auto t_fialCallBack =  [this]( Http * p_http, std::string p_res ){
                    
                    if( s_ajaxPool.find( p_http ) != s_ajaxPool.end() )
                    {
                        
                        auto & t_ajax = s_ajaxPool[ p_http ];
                        std::stringstream t_sstr;
                        t_sstr << t_ajax.fialCallBack << "(\"" << t_ajax.key << "\", " << p_res << ");";
                        std::string t_str = t_sstr.str();
                        m_httpCallBackList.push( [this, t_str](){
                            m_webview->evaluateJS( t_str );
                        });
                        s_ajaxPool.erase( p_http );
                    }
                };
                
                Http * t_http = t_funcList[0].compare( "Post" ) == 0 ? Http::Post( t_ajax.url, &t_parameter, t_successCallBack, t_fialCallBack) : Http::Get( t_ajax.url, &t_parameter, t_successCallBack, t_fialCallBack) ;
                
                s_ajaxPool[ t_http ] = t_ajax;
            }
        }
        
    } );
    
    
    m_webview->setOpacityWebView( 0.0f );
    this->addChild( m_webview );
    
    schedule( schedule_selector(WebViewScene::update) );
    
    return true;
}

void WebViewScene::refreshSource( const DataFileInfo & p_dataInfo )
{
    
}

void WebViewScene::playAudio( const std::string & p_audioUrl, const std::string & p_finishCallBack )
{
    auto t_audioFile = DataTableFile::instance().findBySourceUrl( p_audioUrl );
    if( !t_audioFile.fileId.empty() )
    {
        //播放
        auto t_it = s_playList.find( t_audioFile.fileName );
        if( t_it != s_playList.end() )
        {
            AudioEngine::stop( s_playList[t_audioFile.fileName] );
        }
       
        auto t_playId = AudioEngine::play2d( t_audioFile.fileName );
        s_playList[t_audioFile.sourceUrl] = t_playId;
        
        if( !p_finishCallBack.empty() )
        {
            m_playCallBackList[ t_audioFile.sourceUrl ] = p_finishCallBack;
        }
        
        AudioEngine::setFinishCallback( t_playId, [this]( int p_playId, const std::string p_audio ){
            
            std::string t_audio = "";
            for( auto t_item : s_playList )
            {
                if( t_item.second == p_playId )
                {
                    t_audio = t_item.first;
                    break;
                }
            }
            
            if( t_audio.empty() )
            {
                return;
            }
            
            if( m_playCallBackList.find( t_audio ) != m_playCallBackList.end() && !m_playCallBackList[t_audio].empty() )
            {
                std::stringstream t_sstr;
                t_sstr << m_playCallBackList[t_audio] << "(\"" << t_audio << "\");";
                std::string t_str = t_sstr.str();
                m_webview->evaluateJS( t_sstr.str() );
                m_playCallBackList.erase( t_audio );
            }
            
            s_playList.erase( t_audio );
        });
        return;
    }
    
    m_playCallBackList[p_audioUrl] = p_finishCallBack;
    
    loadAudio( p_audioUrl, [this]( DataFileInfo p_audioFile ){
        playAudio( p_audioFile.sourceUrl, "" );
    });
}

void WebViewScene::stopAudio( const std::string & p_audioUrl )
{
    auto t_it = s_playList.find( p_audioUrl );
    if( t_it != s_playList.end() )
    {
        AudioEngine::stop( s_playList[p_audioUrl] );
    }
}

void WebViewScene::_stopAllAudio( void )
{
    if( !sm_instance )
    {
        return;
    }
    sm_instance->s_downloadList.clear();

    if( sm_instance->s_playList.size() <= 0 )
    {
        return;
    }

    std::stringstream t_sstr;
    for( auto t_item : sm_instance->s_playList )
    {
        AudioEngine::stop( t_item.second );
        t_sstr << sm_instance->m_playCallBackList[t_item.first] << "(\"" << t_item.first << "\");";
    }

    std::string t_str = t_sstr.str();
    sm_instance->m_webview->evaluateJS( t_sstr.str() );

    sm_instance->m_playCallBackList.clear();
    sm_instance->s_playList.clear();
}

void WebViewScene::stopAllAudio( void )
{
    AudioEngine::stopAll();
    s_playList.clear();
    s_downloadList.clear();
}

void WebViewScene::loadAudio( const std::string & p_audioUrl, std::function<void( DataFileInfo p_audioFile )> p_loadAudioCallBack )
{
    
    auto t_audioFile = DataTableFile::instance().findBySourceUrl( p_audioUrl );
    if( !t_audioFile.fileId.empty() && p_loadAudioCallBack )
    {
        p_loadAudioCallBack( t_audioFile );
        return;
    }
    
    for( auto t_item : s_downloadList )
    {
        if( t_item.first.compare( p_audioUrl ) == 0 )
        {
            return;
        }
    }
    
    s_downloadList[ p_audioUrl ] = p_loadAudioCallBack;
    
    
    Http::DownloadFile( p_audioUrl, "mp3", [this]( Http * p_http, DataFileInfo p_fileInfo ){
        
        if( s_downloadList.find( p_fileInfo.sourceUrl ) != s_downloadList.end() && s_downloadList[p_fileInfo.sourceUrl] != nullptr )
        {
            s_downloadList[p_fileInfo.sourceUrl]( p_fileInfo );
        }
        
        auto t_it = s_downloadList.find( p_fileInfo.sourceUrl );
        if( t_it != s_downloadList.end() )
        {
            s_downloadList.erase( t_it );
        }
        
        if( !p_fileInfo.fileId.empty() )
        {
            if( DataTableFile::instance().find( p_fileInfo.fileId ).fileId.empty() )
            {
                DataTableFile::instance().insert( p_fileInfo );
            }else{
                DataTableFile::instance().update( p_fileInfo );
            }
        }
        
    }, [this]( Http * p_http, DataFileInfo p_fileInfo ){
        
        auto t_it = s_downloadList.find( p_fileInfo.sourceUrl );
        if( t_it != s_downloadList.end() )
        {
            s_downloadList.erase( t_it );
        }
    } );
}

void WebViewScene::deleteAudio( const std::string & p_audioUrl )
{
    auto t_audioFile = DataTableFile::instance().findBySourceUrl( p_audioUrl );
    if( !t_audioFile.fileId.empty() )
    {
        DataTableFile::instance().remove( t_audioFile );
        return;
    }
}


std::string WebViewScene::urlRepair( std::string p_url )
{
    
    auto t_it = p_url.find( "://" );
    
    if( t_it != std::string::npos  )
    {
        return p_url;
    }
    
    t_it = p_url.find( "//" );
    
    if( t_it != std::string::npos )
    {
        return p_url.replace( t_it, 2, "://");
    }
    
    return "https://" + p_url;
}

WebViewScene::~WebViewScene()
{
    sm_instance = nullptr;
    std::stringstream t_splistName;
    
    t_splistName << "Web/" << m_dir << ".plist";
    
    if( FileUtils::getInstance()->isFileExist( t_splistName.str() ) )
    {
        SpriteFrameCache::getInstance()->removeSpriteFramesFromFile( t_splistName.str() );
    }
}

void WebViewScene::update( float p_dt )
{
    while( !m_httpCallBackList.empty() )
    {
        auto t_callBack = m_httpCallBackList.top();
        m_httpCallBackList.pop();
        t_callBack();
    }
}
