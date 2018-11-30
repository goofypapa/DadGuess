//
//  DadGuessMainScene.cpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/11/20.
//

#include "DadGuessMainScene.h"
#include "DadGuessMain.hpp"
#include "Config.h"
#include "Common.h"
#include "MainScene.h"
#include "DadGuessUpdateScene.h"
#include "DadGuessCardListScene.h"

#include "DataTableCard.h"
#include "DataTableCardAudio.h"

#include "ui/CocosGUI.h"

#include "AudioEngine.h"
#include "Http.h"

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace experimental;
using namespace rapidjson;

const char * DadGuessMainScene::sm_cardAudioInfoApi = DOMAIN_NAME "api/card/app/get.do";
const char * DadGuessMainScene::sm_cardAudioInfoApiAnimal = DOMAIN_NAME "api/card/res/get.do";

BlueDeviceListener * DadGuessMainScene::sm_blueDeviceScanCardListener = nullptr;
bool DadGuessMainScene::sm_blueState = false;

bool DadGuessMainScene::init( void )
{
    if( !BaseScene::init() )
    {
        return false;
    }
    
    auto t_visibleSizeHalf = Director::getInstance()->getVisibleSize() * 0.5f;
    Vec2 t_origin = Director::getInstance()->getVisibleOrigin();
    
    auto t_center = Point( t_origin.x + t_visibleSizeHalf.width, t_origin.y + t_visibleSizeHalf.height );
    
    auto t_background = TexturePacker::DadGuessMain::createCaicai_home_bjSprite();
    
    t_background->setPosition( Vec2( t_visibleSizeHalf.width + t_origin.x, t_visibleSizeHalf.height + t_origin.y ) );
    auto t_backgroundSizeHalf = t_background->getContentSize() * 0.5f;
    
    t_background->setScale( t_visibleSizeHalf.width / t_backgroundSizeHalf.width, t_visibleSizeHalf.height / t_backgroundSizeHalf.height );
    
    addChild( t_background, 0 );
    
    auto t_homeButton = Button::create( TexturePacker::DadGuessMain::caicai_home_icon_gohome, TexturePacker::DadGuessMain::caicai_home_icon_gohome, "", Button::TextureResType::PLIST );
    t_homeButton->setScale( adaptation() );
    auto t_homeButtonSizeHalf = t_homeButton->getContentSize() * t_homeButton->getScale() * 0.5f;
    
    t_homeButton->setPosition( Vec2( t_origin.x + t_homeButtonSizeHalf.width + 15.0f, t_origin.y + t_visibleSizeHalf.height * 2.0f - t_homeButtonSizeHalf.height - 15.0f ) );
    
    addChild( t_homeButton );
    
    touchAnswer( t_homeButton, [this]( Ref * p_target ){
        Director::getInstance()->replaceScene( MainScene::create() );
        DadGuessUpdateScene::unCacheResource();
        destroy();
    }, adaptation() * 1.2f, adaptation());
    
    auto t_logo = TexturePacker::DadGuessMain::createCaicai_home_pic_logoSprite();
    t_logo->setScale( adaptation() );
    auto t_logoSizeHalf = t_logo->getContentSize() * t_logo->getScale() * 0.5f;
    
    t_logo->setPosition( Vec2( t_origin.x + t_visibleSizeHalf.width, t_origin.y + t_visibleSizeHalf.height * 2.0f - t_logoSizeHalf.height - 5.0f ) );
    
    addChild( t_logo );
    
    m_blueConnectState = sm_blueState ? TexturePacker::DadGuessMain::createCaicai_home_icon_blueteeth_yesSprite() : TexturePacker::DadGuessMain::createCaicai_home_icon_blueteeth_noSprite();
    
    m_blueConnectState->setScale( adaptation() );
    auto t_blueConnectStateSizeHalf = m_blueConnectState->getContentSize() * m_blueConnectState->getScale() * 0.5f;
    m_blueConnectState->setPosition( Vec2( t_origin.x + t_visibleSizeHalf.width * 2.0f - t_blueConnectStateSizeHalf.width - 15.0f, t_origin.y + t_visibleSizeHalf.height * 2.0f - t_blueConnectStateSizeHalf.height - 15.0f ) );
    addChild( m_blueConnectState );
    
    auto t_product = TexturePacker::DadGuessMain::createCaicai_home_pic_productSprite();
    auto t_productSizeHalf = t_product->getContentSize() * 0.5f;
    
    float t_adaptation = t_visibleSizeHalf.height * 0.35f / t_productSizeHalf.height;
    t_product->setScale( t_adaptation );
    
    t_product->setPosition( Vec2( t_center.x, t_origin.y + t_productSizeHalf.height * t_product->getScale() ) );
    addChild( t_product );
    
    //
    auto t_btnAnimal = Button::create( TexturePacker::DadGuessMain::caicai_home_animal, TexturePacker::DadGuessMain::caicai_home_animal, "", Button::TextureResType::PLIST );
    t_btnAnimal->setScale( t_adaptation );
    auto t_btnAnimalSizeHalf = t_btnAnimal->getContentSize() * t_btnAnimal->getScale() * 0.5f;
    
    t_btnAnimal->setPosition( Vec2( t_center.x - t_btnAnimalSizeHalf.width - 15.0f, t_center.y + t_btnAnimalSizeHalf.height  + 22.0f ) );
    addChild( t_btnAnimal );
    
    //
    auto t_btnAbc = Button::create( TexturePacker::DadGuessMain::caicai_home_abc, TexturePacker::DadGuessMain::caicai_home_abc, "", Button::TextureResType::PLIST );
    t_btnAbc->setScale( t_adaptation );
    auto t_btnAbcSizeHalf = t_btnAbc->getContentSize() * t_btnAbc->getScale() * 0.5f;
    
    t_btnAbc->setPosition( Vec2( t_center.x + t_btnAbcSizeHalf.width - 3.0f, t_center.y  + t_btnAbcSizeHalf.height + 15.0f ) );
    addChild( t_btnAbc );
    
    //
    auto t_btnSky = Button::create( TexturePacker::DadGuessMain::caicai_home_sky, TexturePacker::DadGuessMain::caicai_home_sky, "", Button::TextureResType::PLIST );
    t_btnSky->setScale( t_adaptation  );
    auto t_btnSkySizeHalf = t_btnSky->getContentSize() * t_btnSky->getScale() * 0.5f;
    
    t_btnSky->setPosition( Vec2( t_center.x - t_btnSkySizeHalf.width * 3.0f - 25.0f, t_center.y + 10.0f ) );
    addChild( t_btnSky );
    
    //
    auto t_btnEart = Button::create( TexturePacker::DadGuessMain::caicai_home_earth, TexturePacker::DadGuessMain::caicai_home_earth, "", Button::TextureResType::PLIST );
    t_btnEart->setScale( t_adaptation  );
    auto t_btnEartSizeHalf = t_btnEart->getContentSize() * t_btnEart->getScale() * 0.5f;
    
    t_btnEart->setPosition( Vec2( t_center.x + t_btnEartSizeHalf.width * 3.0f + 3.0f, t_center.y + 10.0f ) );
    addChild( t_btnEart );
    
    //
    auto t_btnHistory = Button::create( TexturePacker::DadGuessMain::caicai_home_history, TexturePacker::DadGuessMain::caicai_home_history, "", Button::TextureResType::PLIST );
    t_btnHistory->setScale( t_adaptation  );
    auto t_btnHistorySizeHalf = t_btnHistory->getContentSize() * t_btnHistory->getScale() * 0.5f;
    
    t_btnHistory->setPosition( Vec2( t_center.x - t_btnHistorySizeHalf.width - 20.0f, t_center.y  - t_btnHistorySizeHalf.height + 4.0f ) );
    addChild( t_btnHistory );
    
    //
    auto t_btnChineseHistory = Button::create( TexturePacker::DadGuessMain::caicai_home_chinese, TexturePacker::DadGuessMain::caicai_home_chinese, "", Button::TextureResType::PLIST );
    t_btnChineseHistory->setScale( t_adaptation );
    auto t_btnChineseHistorySizeHalf = t_btnChineseHistory->getContentSize() * t_btnChineseHistory->getScale() * 0.5f;
    
    t_btnChineseHistory->setPosition( Vec2( t_center.x + t_btnChineseHistorySizeHalf.width, t_center.y  - t_btnChineseHistorySizeHalf.height + 7.0f ) );
    addChild( t_btnChineseHistory );
    
    //
    auto t_btnTouched = [this]( Ref * p_target ){
        int t_tag = ((Button *)p_target)->getTag();
        
        Director::getInstance()->replaceScene( DadGuessCardListScene::createScene( DataCardBatchInfo::s_batchIdList[t_tag] ) );
        destroy();
    };
    
    Button * t_btnList[] = { t_btnAnimal, t_btnAbc, t_btnSky, t_btnEart, t_btnHistory, t_btnChineseHistory };
    
    for( int i = 0; i < sizeof( t_btnList ) / sizeof( Button * ); ++i )
    {
        t_btnList[i]->setTag( i );
        touchAnswer( t_btnList[i], t_btnTouched, t_adaptation * 1.2f, t_adaptation );
    }
    
    BlueDeviceListener::listen();
    
    if( sm_blueDeviceScanCardListener == nullptr )
    {
        sm_blueDeviceScanCardListener = new BlueDeviceListener( [this]( bool p_connected ){
            printf( "--------> connected: %s \n", ( p_connected ? "true" : "false" ) );
            sm_blueState = p_connected;
        }, [this]( const std::vector< unsigned char > & p_data ){
            
            static std::vector< unsigned char > s_lastData;
            
            bool t_isEqual = false;
            if( s_lastData.size() == p_data.size() )
            {
                t_isEqual = true;
                for( int i = 0; i < s_lastData.size(); ++i )
                {
                    if( s_lastData[ i ] != p_data[ i ] )
                    {
                        t_isEqual = false;
                        break;
                    }
                }
            }
            
            if( t_isEqual )
            {
                return;
            }
            s_lastData = p_data;
            
            if( p_data[0] == 0xAB && p_data[1] == 0x03 )
            {
                int t_rfid = p_data[2] * 256 + p_data[3];
                
                DadGuessMainScene::scanCard( t_rfid );
            }
        } );
    }
    
    m_blueDeviceConnectedListener = new BlueDeviceListener( [this]( bool p_connected ){
        m_blueConnectState->setSpriteFrame( p_connected ? TexturePacker::DadGuessMain::caicai_home_icon_blueteeth_yes : TexturePacker::DadGuessMain::caicai_home_icon_blueteeth_no );

    }, nullptr );

    return true;
}

void DadGuessMainScene::destroy( void )
{
    delete m_blueDeviceConnectedListener;
}

void DadGuessMainScene::scanCard( int p_rfid )
{
    auto t_cardInfo = DataTableCard::instance().find( p_rfid );
    
    if( t_cardInfo.id.empty() )
    {
        return;
    }
    
    auto t_cardBatchInfo = DataTableCardBatch::instance().find( t_cardInfo.batchId );
    
    
    //激活分组
//    if( !t_cardBatchInfo.activation )
//    {
//        DataTableCardBatch::instance().activation( t_cardBatchInfo );
//    }
    
    // 卡片未激活
    if( !t_cardInfo.activation )
    {
        static std::map< Http *, DataCardAudioInfo > s_downloadAudioList;
        static std::map< std::string, std::vector< std::pair< std::string , DataCardAudioInfo> > > s_notActivationCardList;
        //查看缓存
        
        if( s_notActivationCardList.find( t_cardInfo.id ) != s_notActivationCardList.end() )
        {
            auto & t_audioUrlList = s_notActivationCardList[t_cardInfo.id];
            
            int t_index = (int)t_audioUrlList.size();
            
            if( t_index == 0 )
            {
                return;
            }
            
            while ( t_index == t_audioUrlList.size() ) {
                t_index = (int)floor( rand_0_1() * t_audioUrlList.size() );
            }
            
            auto t_audioUrl = t_audioUrlList[t_index].first;
            auto t_audioInfo = t_audioUrlList[t_index].second;
            auto t_audioFileInfo = DataTableFile::instance().findBySourceUrl( t_audioUrl );
            if( !t_audioFileInfo.fileId.empty() )
            {
                t_audioInfo.fileId = t_audioFileInfo.fileId;
                playAudio( t_audioInfo );
                return;
            }
            
            auto t_http = Http::DownloadFile( t_audioUrl, "", [=]( Http * p_http, DataFileInfo p_fileInfo ){
                
                if( s_downloadAudioList.find( p_http ) == s_downloadAudioList.end() )
                {
                    return;
                }
                
                auto t_audioInfo = s_downloadAudioList[p_http];
                t_audioInfo.fileId = p_fileInfo.fileId;
                
                DataTableCardAudio::instance().insert( t_audioInfo );
                
                playAudio( t_audioInfo );
                
            }, [=]( Http * p_http, DataFileInfo p_fileInfo ){
                
            } );
            if( t_http )
            {
                s_downloadAudioList[ t_http ] = t_audioInfo;
            }
            return;
        }
        Http::HttpParameter t_httpParameter;
        t_httpParameter["RFID"] =  std::to_string( p_rfid );
        
        printf( "------rfid: %d \n", p_rfid );
        
        if( t_cardInfo.batchId.compare( DataCardBatchInfo::s_batchIdList[0] ) != 0 )
        {
            
            Http::Post( sm_cardAudioInfoApi, &t_httpParameter, [=]( Http * p_http, const std::string & p_res ){
                Document t_readdoc;
                
                t_readdoc.Parse<0>( p_res.c_str() );
                
                if( t_readdoc.HasParseError() )
                {
                    printf( "GetParseError %d \n", t_readdoc.GetParseError() );
                }
                
                auto t_success = t_readdoc["success"].GetBool();
                if( !t_success )
                {
                    printf( "------------> 检查更新失败" );
                    return;
                }
                
                auto & t_data = t_readdoc["data"];
                auto t_cardId = t_data["cardId"].GetString();
                auto & t_dataAudios = t_data["audios"];
                if( t_dataAudios.IsArray() )
                {
                    std::vector< std::pair< std::string , DataCardAudioInfo> > t_audioList;
                    for( int i = 0; i < t_dataAudios.Capacity(); ++i )
                    {
                        auto t_dataAudioUrl = std::string( DOMAIN_NAME ) + t_dataAudios[i]["attUrl"].GetString();

                        t_audioList.push_back( std::pair< std::string , DataCardAudioInfo>( t_dataAudioUrl, DataCardAudioInfo( createUUID(), t_cardId, "", DataCardAudioInfo::AudioType::commentary ) ) );
                    }
                    
                    s_notActivationCardList[ t_cardId ] = t_audioList;
                    
                    scanCard( p_rfid );
                }
                
            }, [=]( Http * p_http, const std::string & p_res ){
                printf( "---------> request card info fial %s \n", p_res.c_str() );
            } );
        }else{
            //获取动物卡片音频
            Http::Post( sm_cardAudioInfoApiAnimal, &t_httpParameter, [=]( Http * p_http, const std::string & p_res ){
                
                Document t_readdoc;
                
                t_readdoc.Parse<0>( p_res.c_str() );
                
                if( t_readdoc.HasParseError() )
                {
                    printf( "GetParseError %d \n", t_readdoc.GetParseError() );
                }
                
                auto t_success = t_readdoc["success"].GetBool();
                if( !t_success )
                {
                    printf( "------------> 检查更新失败" );
                    return;
                }
                
                auto & t_data = t_readdoc["data"];
                auto t_cardId = t_data["resourceId"].GetString();
                
                
                std::vector< std::pair< std::string , DataCardAudioInfo> > t_audioList;
                auto & t_dataAudios = t_data["audios"];
                if( t_dataAudios.IsArray() )
                {
                    for( int i = 0; i < t_dataAudios.Capacity(); ++i )
                    {
                        auto t_dataAudioUrl = std::string( DOMAIN_NAME ) + t_dataAudios[i]["attUrl"].GetString();
                        
                        t_audioList.push_back( std::pair< std::string , DataCardAudioInfo>( t_dataAudioUrl, DataCardAudioInfo( createUUID(), t_cardId, "", DataCardAudioInfo::AudioType::hue ) ) );
                    }
                }
                
                auto & t_dataDescAudios = t_data["descAudio"];
                
                if( t_dataDescAudios.IsArray() )
                {
                    for( int i = 0; i < t_dataDescAudios.Capacity(); ++i )
                    {
                        auto t_dataAudioUrl = std::string( DOMAIN_NAME ) + t_dataDescAudios[i]["attUrl"].GetString();
                        
                        t_audioList.push_back( std::pair< std::string , DataCardAudioInfo>( t_dataAudioUrl, DataCardAudioInfo( createUUID(), t_cardId, "", DataCardAudioInfo::AudioType::commentary ) ) );
                    }
                }
                
                auto & t_dataPronAudio = t_data["pronAudio"];
                
                if( t_dataPronAudio.IsObject() )
                {
                    auto t_dataAudioUrl = std::string( DOMAIN_NAME ) + t_dataPronAudio["attUrl"].GetString();
                    
                    t_audioList.push_back( std::pair< std::string , DataCardAudioInfo>( t_dataAudioUrl, DataCardAudioInfo( createUUID(), t_cardId, "", DataCardAudioInfo::AudioType::commentary ) ) );
                }
                
                s_notActivationCardList[ t_cardId ] = t_audioList;
                
                scanCard( p_rfid );
                
            }, [=]( Http * p_http, const std::string & p_res ){
                printf( "---------> request card info fial %s \n", p_res.c_str() );
            } );
        }
        
        //下载音频
        printf( "---------> batchId: %s \n", t_cardInfo.batchId.c_str() );
        return;
    }
    
    auto t_audioList = DataTableCardAudio::instance().list( t_cardInfo.id );
    
    int t_index = (int)t_audioList.size();
    
    if( t_index == 0 )
    {
        return;
    }
    
    while ( t_index == t_audioList.size() ) {
        t_index = (int)floor( rand_0_1() * t_audioList.size() );
    }
    
    auto t_audioInfo = t_audioList[t_index];
    playAudio( t_audioInfo );
}

void DadGuessMainScene::playAudio( const DataCardAudioInfo & p_audioInfo )
{
    static int s_soloPlayId = -1;
    static DataCardAudioInfo s_prvePlayAudio;
    
    auto t_audioFileInfo = DataTableFile::instance().find( p_audioInfo.fileId );
    if( p_audioInfo.audioType == DataCardAudioInfo::AudioType::commentary )
    {
        AudioEngine::stopAll();
        s_soloPlayId = AudioEngine::play2d( t_audioFileInfo.fileName );
        s_prvePlayAudio = p_audioInfo;
    }else{
        
        if( p_audioInfo.cardId.compare( s_prvePlayAudio.cardId ) != 0 && AudioEngine::getState( s_soloPlayId ) == AudioEngine::AudioState::PLAYING )
        {
            AudioEngine::stop( s_soloPlayId );
        }
        AudioEngine::play2d( t_audioFileInfo.fileName );
    }
}
