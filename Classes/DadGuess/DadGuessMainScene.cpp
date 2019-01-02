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
#include "Main.hpp"
#include "Dialog.hpp"
#include "MainScene.h"
#include "DadGuessUpdateScene.h"
#include "DadGuessCardListScene.h"
#include "DadGuessBlueScene.h"
#include "DataTableFile.h"
#include "BlueDeviceListener.h"
#include "WebViewScene.h"

#include "DataTableCard.h"
#include "DataTableCardAudio.h"
#include "DataTableKeyValue.h"

#include "AudioEngine.h"

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

#define PAGE_FONT "fonts/HuaKangFangYuanTIW7-GB_0.ttf"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace experimental;
using namespace rapidjson;

const char * DadGuessMainScene::sm_cardAudioInfoApi = DOMAIN_NAME "/api/card/app/get.do";
const char * DadGuessMainScene::sm_cardAudioInfoApiAnimal = DOMAIN_NAME "/api/card/res/get.do";
const char * DadGuessMainScene::sm_checkoutCardAudioUpdateKey = "CheckoutCardAudioUpdate";
const long DadGuessMainScene::sm_checkoutCardAudioUpdateOverTime = 30 * 60 * 60 * 24 * 2;

BlueDeviceListener * DadGuessMainScene::sm_blueDeviceScanCardListener = nullptr;
bool DadGuessMainScene::sm_blueState = false;

Http * DadGuessMainScene::sm_lastDownload = nullptr;

bool DadGuessMainScene::init( void )
{
    if( !BaseScene::init() )
    {
        return false;
    }
    
    m_loginUser = DataTableUser::instance().getActivation();
    //加载贴图
    TexturePacker::Main::addSpriteFramesToCache();
    TexturePacker::Dialog::addSpriteFramesToCache();
    
    m_enableMainButton = true;
    
    auto t_visibleSizeHalf = Director::getInstance()->getVisibleSize() * 0.5f;
    Vec2 t_origin = Director::getInstance()->getVisibleOrigin();
    
    auto t_center = Point( t_origin.x + t_visibleSizeHalf.width, t_origin.y + t_visibleSizeHalf.height );
    
    auto t_background = TexturePacker::DadGuessMain::createCaicai_home_bjSprite();
    
    t_background->setPosition( Vec2( t_visibleSizeHalf.width + t_origin.x, t_visibleSizeHalf.height + t_origin.y ) );
    auto t_backgroundSizeHalf = t_background->getContentSize() * 0.5f;
    
    t_background->setScale( t_visibleSizeHalf.width / t_backgroundSizeHalf.width, t_visibleSizeHalf.height / t_backgroundSizeHalf.height );
    
    addChild( t_background, 0 );
    
    
    auto t_userHeadBorder = Button::create( TexturePacker::Main::mainUserHeadBackground, TexturePacker::Main::mainUserHeadBackground, "", Widget::TextureResType::PLIST );
    auto t_userHeadBorderSizeHalf = t_userHeadBorder->getContentSize() * 0.5f;
    auto t_userHeadBorderPosition = Vec2( t_origin.x + t_userHeadBorderSizeHalf.width * adaptation() + 15.0f , t_origin.y + t_visibleSizeHalf.height * 2.0f - t_userHeadBorderSizeHalf.height * adaptation() - 10.0f );
    
    t_userHeadBorder->setPosition( t_userHeadBorderPosition );
    t_userHeadBorder->setScale( adaptation() );
    this->addChild( t_userHeadBorder );
    
    
    touchAnswer( t_userHeadBorder , [this]( Ref * p_ref ){
        
        if( !m_enableMainButton ) return;
        
        m_enableMainButton = false;
        m_dialogPersonalCenter->show();
    }, adaptation() * 1.1f, adaptation() );
    
    auto t_fileInfo = DataTableFile::instance().find( m_loginUser.headImg );
    
    m_personalHead = t_fileInfo.fileId.empty() ? Sprite::create( "DefaultHead.png" ) : Sprite::create( t_fileInfo.fileName ) ;
    
    auto t_personalHeadSizeHelf = m_personalHead->getContentSize() * 0.5f;
    m_personalHead->setPosition( Vec2( t_userHeadBorderSizeHalf.width, t_userHeadBorderSizeHalf.height ) );
    m_personalHead->setScale( t_userHeadBorderSizeHalf.height * 0.93f / t_personalHeadSizeHelf.height );
    t_userHeadBorder->addChild( m_personalHead );
    
    GLProgram * t_userHeadProgram = GLProgramCache::getInstance()->getGLProgram( "UserHead" );
    if( !t_userHeadProgram )
    {
        t_userHeadProgram = GLProgram::createWithFilenames( "UserHead.vert", "UserHead.frag" );
        GLProgramCache::getInstance()->addGLProgram( t_userHeadProgram, "UserHead" );
    }
    
    GLProgramState * t_userHeadProgramState = GLProgramState::getOrCreateWithGLProgram( t_userHeadProgram );
    m_personalHead->setGLProgramState( t_userHeadProgramState );
    
    auto t_personalName = Label::createWithTTF( m_loginUser.userName, PAGE_FONT, 16 );
    
    t_personalName->setAlignment( TextHAlignment::LEFT );
    t_personalName->setAnchorPoint( Point( 0.0f, 0.5f ) );
    
    t_personalName->setPosition( Vec2( t_userHeadBorderPosition.x + t_userHeadBorderSizeHalf.width * adaptation() + 5.0f, t_userHeadBorderPosition.y ) );
    
    this->addChild( t_personalName );
    
    
//    auto t_homeButton = Button::create( TexturePacker::DadGuessMain::caicai_home_icon_gohome, TexturePacker::DadGuessMain::caicai_home_icon_gohome, "", Button::TextureResType::PLIST );
//    t_homeButton->setScale( adaptation() );
//    auto t_homeButtonSizeHalf = t_homeButton->getContentSize() * t_homeButton->getScale() * 0.5f;
//
//    t_homeButton->setPosition( Vec2( t_origin.x + t_homeButtonSizeHalf.width + 15.0f, t_origin.y + t_visibleSizeHalf.height * 2.0f - t_homeButtonSizeHalf.height - 15.0f ) );
//
//    addChild( t_homeButton );
    
//    touchAnswer( t_homeButton, [this]( Ref * p_target ){
//        Director::getInstance()->replaceScene( MainScene::create() );
//        DadGuessUpdateScene::unCacheResource();
//    }, adaptation() * 1.2f, adaptation());
    
    auto t_logo = TexturePacker::DadGuessMain::createCaicai_home_pic_logoSprite();
    t_logo->setScale( adaptation() );
    auto t_logoSizeHalf = t_logo->getContentSize() * t_logo->getScale() * 0.5f;
    
    t_logo->setPosition( Vec2( t_origin.x + t_visibleSizeHalf.width, t_origin.y + t_visibleSizeHalf.height * 2.0f - t_logoSizeHalf.height - 5.0f ) );
    
    addChild( t_logo );


    m_blueConnectState = Button::create( sm_blueState ? TexturePacker::DadGuessMain::caicai_home_icon_blueteeth_yes : TexturePacker::DadGuessMain::caicai_home_icon_blueteeth_no , "", "", Button::TextureResType::PLIST );

    m_blueConnectState->setScale( adaptation() );
    auto t_blueConnectStateSizeHalf = m_blueConnectState->getContentSize() * m_blueConnectState->getScale() * 0.5f;
    m_blueConnectState->setPosition( Vec2( t_origin.x + t_visibleSizeHalf.width * 2.0f - t_blueConnectStateSizeHalf.width - 15.0f, t_origin.y + t_visibleSizeHalf.height * 2.0f - t_blueConnectStateSizeHalf.height - 15.0f ) );
    addChild( m_blueConnectState );

    touchAnswer( m_blueConnectState, [this]( Ref * p_target ){
        if( !m_enableMainButton ) return;
        printf( "----------> %s \n", sm_blueState ? "connected" : "not connected" );

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        goSystemBlue();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        if( !sm_blueState )
        {
            Director::getInstance()->replaceScene( DadGuessBlueScene::create() );
        }else{
            BlueDeviceListener::deconnect();
        }
#endif
        

    }, adaptation() * 1.2f, adaptation() );
    
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
        if( !m_enableMainButton ) return;
        int t_tag = ((Button *)p_target)->getTag();
        
        Director::getInstance()->replaceScene( DadGuessCardListScene::createScene( DataCardBatchInfo::s_batchIdList[t_tag] ) );
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
            if( !sm_blueState )
            {
                AudioEngine::stopAll();
            }
        }, [this]( const std::vector< unsigned char > & p_data ){
            static std::map< int, bool > s_scanCardList;
            
            if( p_data[0] == 0xAB && ( p_data[1] == 0x03 || p_data[1] == 0x04 ) )
            {
                int t_rfid = ( p_data[2] << 8 ) + p_data[3];

                if( p_data[1] == 0x03 )
                {
                    if( s_scanCardList.find( t_rfid ) == s_scanCardList.end() )
                    {
                        s_scanCardList[t_rfid] = false;
                    }

                    if( s_scanCardList[t_rfid] )
                    {
                        return;
                    }
                    s_scanCardList[t_rfid] = true;
                    DadGuessMainScene::scanCard( t_rfid );
                }

                if( p_data[1] == 0x04 )
                {
                    // s_scanCardList[t_rfid] = false;
                    s_scanCardList.clear();
                }
            }
        } );
    }
    
    m_blueDeviceConnectedListener = new BlueDeviceListener( [this]( bool p_connected ){
        m_blueConnectState->loadTextureNormal( sm_blueState ? TexturePacker::DadGuessMain::caicai_home_icon_blueteeth_yes : TexturePacker::DadGuessMain::caicai_home_icon_blueteeth_no, Button::TextureResType::PLIST );
    }, nullptr );
    
    
    //用户中心
    m_dialogPersonalCenter = DialogPersonalCenterLayer::create();
    m_dialogPersonalCenter->setPosition( Vec2( t_visibleSizeHalf.width + t_origin.x, t_visibleSizeHalf.height + t_origin.y ) );
    this->addChild( m_dialogPersonalCenter );
    
    m_dialogPersonalCenter->hideCallBack = [this, t_personalName](){
        m_enableMainButton = true;
        auto t_userInfo = DataTableUser::instance().getActivation();
        
        t_personalName->initWithTTF( t_userInfo.userName, PAGE_FONT, 16 );
        
        updateUserInfo();
        
    };

    return true;
}

DadGuessMainScene::~DadGuessMainScene( void )
{
    delete m_blueDeviceConnectedListener;
}

void DadGuessMainScene::scanCard( int p_rfid )
{

    static std::map< std::string, std::vector< DataCardAudioInfo > > s_cardAudioPool;
    
    printf( "-------> scan: %d \n", p_rfid );
    
    auto t_cardInfo = DataTableCard::instance().find( p_rfid );

    if( t_cardInfo.id.empty() )
    {
        return;
    }
    
    auto t_cardBatchInfo = DataTableCardBatch::instance().find( t_cardInfo.batchId );
    
    //激活分组
    if( !t_cardBatchInfo.activation )
    {
        DataTableCardBatch::instance().activation( t_cardBatchInfo );
    }

    if( s_cardAudioPool.find( t_cardInfo.id ) == s_cardAudioPool.end() )
    {
        bool t_needCheckAudioUpdate = true;
        if( t_cardInfo.activation )
        {
            // time_t t_curTime;
            // time(&t_curTime);
            
            // std::stringstream t_sstr;
            // t_sstr << sm_checkoutCardAudioUpdateKey << t_cardInfo.id;
            
            // auto t_keyValueInfo = DataTableKeyValue::instance().get( t_sstr.str() );

            // if( !t_keyValueInfo.key.empty() && t_keyValueInfo.getBooleanValue() && t_curTime - t_keyValueInfo.date < sm_checkoutCardAudioUpdateOverTime )
            // {
                t_needCheckAudioUpdate = false;
            // }
        }

        if( t_needCheckAudioUpdate )
        {
            std::stringstream t_sstr;
            t_sstr << sm_checkoutCardAudioUpdateKey << t_cardInfo.id;
            DataTableKeyValue::instance().set( DataKeyValueInfo( t_sstr.str(), false ) );
            Http::HttpParameter t_httpParameter;
            t_httpParameter["RFID"] =  std::to_string( p_rfid );
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
                        printf( "------------> 服务器错误\n" );
                        return;
                    }
                    
                    auto & t_data = t_readdoc["data"];
                    auto t_cardId = t_data["cardId"].GetString();
                    auto & t_dataAudios = t_data["audios"];

                    if( s_cardAudioPool.find( t_cardId ) == s_cardAudioPool.end() )
                    {
                        s_cardAudioPool[ t_cardId ] = std::vector< DataCardAudioInfo >();
                    }

                    auto t_cardAudioList = DataTableCardAudio::instance().list( t_cardId );
                    if( t_dataAudios.IsArray() )
                    {
                        for( int i = 0; i < t_dataAudios.Capacity(); ++i )
                        {
                            auto t_dataAudioUrl = std::string( DOMAIN_NAME ) + "/" + std::string( t_dataAudios[i]["attUrl"].GetString() ).substr( 1 );
                            auto t_dataAudioMd5 = t_dataAudios[i]["md5"].GetString();
                            auto t_cardAudioInfo = DataCardAudioInfo( "", t_cardId, t_dataAudioUrl, t_dataAudioMd5, DataCardAudioInfo::AudioType::commentary );
                            

                            auto t_findCardAudioIt = t_cardAudioList.begin();
                            while( t_findCardAudioIt != t_cardAudioList.end() )
                            {
                                if( t_findCardAudioIt->fileUrl.compare( t_cardAudioInfo.fileUrl ) == 0 )
                                {
                                    break;
                                }

                                t_findCardAudioIt++;
                            }

                            if( t_findCardAudioIt != t_cardAudioList.end() )
                            {
                                t_cardAudioInfo.id = t_findCardAudioIt->id;
                                DataTableCardAudio::instance().update( t_cardAudioInfo );
                                t_cardAudioList.erase( t_findCardAudioIt );
                            }else{
                                t_cardAudioInfo.id = createUUID();
                                DataTableCardAudio::instance().insert( t_cardAudioInfo );
                            }


                            s_cardAudioPool[ t_cardId ].push_back( t_cardAudioInfo );
                        }
                        
                        for( auto t_removeCardAudioInfo : t_cardAudioList )
                        {
                            DataTableCardAudio::instance().remove( t_removeCardAudioInfo.id );
                        }

                        std::stringstream t_sstr;
                        t_sstr << sm_checkoutCardAudioUpdateKey << std::string( t_cardId );
                        DataTableKeyValue::instance().set( DataKeyValueInfo( t_sstr.str(), true ) );
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
                    
                    auto t_cardAudioList = DataTableCardAudio::instance().list( t_cardId );

                    auto & t_dataAudios = t_data["audios"];

                    std::vector< DataCardAudioInfo > t_tmpAudioList;

                    if( t_dataAudios.IsArray() )
                    {
                        for( int i = 0; i < t_dataAudios.Capacity(); ++i )
                        {
                            auto t_dataAudioUrl = std::string( DOMAIN_NAME ) + "/" + t_dataAudios[i]["attUrl"].GetString();
                            auto t_dataAudioMd5 = t_dataAudios[i]["md5"].GetString();
                            auto t_cardAudioInfo = DataCardAudioInfo( "", t_cardId, t_dataAudioUrl, t_dataAudioMd5, DataCardAudioInfo::AudioType::hue );
                            t_tmpAudioList.push_back( t_cardAudioInfo );
                        }
                    }
                    
                    auto & t_dataDescAudios = t_data["descAudio"];
                    
                    if( t_dataDescAudios.IsArray() )
                    {
                        for( int i = 0; i < t_dataDescAudios.Capacity(); ++i )
                        {
                            auto t_dataAudioUrl = std::string( DOMAIN_NAME ) + "/" + t_dataDescAudios[i]["attUrl"].GetString();
                            auto t_dataAudioMd5 = t_dataDescAudios[i]["md5"].GetString();
                            auto t_cardAudioInfo = DataCardAudioInfo( "", t_cardId, t_dataAudioUrl, t_dataAudioMd5, DataCardAudioInfo::AudioType::commentary );
                            t_tmpAudioList.push_back( t_cardAudioInfo );
                        }
                    }
                    
                    auto & t_dataPronAudio = t_data["pronAudio"];
                    
                    if( t_dataPronAudio.IsObject() )
                    {
                        auto t_dataAudioUrl = std::string( DOMAIN_NAME ) + "/" + t_dataPronAudio["attUrl"].GetString();
                        auto t_dataAudioMd5 = t_dataPronAudio["md5"].GetString();
                        auto t_cardAudioInfo = DataCardAudioInfo( "", t_cardId, t_dataAudioUrl, t_dataAudioMd5,  DataCardAudioInfo::AudioType::commentary );
                        t_tmpAudioList.push_back( t_cardAudioInfo );
                    }

                    for( auto t_cardAudioInfo : t_tmpAudioList )
                    {
                        auto t_findCardAudioIt = t_cardAudioList.begin();
                        while( t_findCardAudioIt != t_cardAudioList.end() )
                        {
                            if( t_findCardAudioIt->fileUrl.compare( t_cardAudioInfo.fileUrl ) == 0 )
                            {
                                break;
                            }

                            t_findCardAudioIt++;
                        }

                        if( t_findCardAudioIt != t_cardAudioList.end() )
                        {
                            t_cardAudioInfo.id = t_findCardAudioIt->id;
                            DataTableCardAudio::instance().update( t_cardAudioInfo );
                            t_cardAudioList.erase( t_findCardAudioIt );
                        }else{
                            t_cardAudioInfo.id = createUUID();
                            DataTableCardAudio::instance().insert( t_cardAudioInfo );
                        }

                        s_cardAudioPool[ t_cardId ].push_back( t_cardAudioInfo );
                    }

                    for( auto t_removeCardAudioInfo : t_cardAudioList )
                    {
                        DataTableCardAudio::instance().remove( t_removeCardAudioInfo.id );
                    }
                    std::stringstream t_sstr;
                    t_sstr << sm_checkoutCardAudioUpdateKey << std::string( t_cardId );
                    DataTableKeyValue::instance().set( DataKeyValueInfo( t_sstr.str(), true ) );
                    scanCard( p_rfid );
                    
                }, [=]( Http * p_http, const std::string & p_res ){
                    printf( "---------> request card info fial %s \n", p_res.c_str() );
                } );
            }
            return;
        }

        s_cardAudioPool[ t_cardInfo.id ] = DataTableCardAudio::instance().list( t_cardInfo.id );

        printf( "------------ %s: %d \n", t_cardInfo.id.c_str(), s_cardAudioPool[ t_cardInfo.id ].size() );

    }
    
    if( !t_cardInfo.activation )
    {
        DataTableCard::instance().activation( t_cardInfo );
    }
    
    auto t_audioList = s_cardAudioPool[ t_cardInfo.id ];

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

    static std::map< Http *, DataCardAudioInfo > s_downloadPool;
    
    auto t_audioFileInfo = DataTableFile::instance().findBySourceUrl( p_audioInfo.fileUrl );

    if( t_audioFileInfo.fileId.empty() )
    {
        auto t_http = Http::DownloadFile( p_audioInfo.fileUrl, "", [=]( Http * p_http, DataFileInfo p_fileInfo ){
            if( sm_lastDownload == p_http ){
                playAudio( s_downloadPool[ p_http ] );
            }
        }, [=]( Http * p_http, DataFileInfo p_fileInfo ){
            printf( " download audio fial \n" );
        } );
        sm_lastDownload = t_http;
        s_downloadPool[ t_http ] = p_audioInfo;
        return;
    }

    WebViewScene::_stopAllAudio();

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

void DadGuessMainScene::updateUserInfo( void )
{
    m_loginUser = DataTableUser::instance().getActivation();
}

void DadGuessMainScene::refreshSource( const DataFileInfo & p_fileInfo )
{
    auto t_loginUser = DataTableUser::instance().getActivation();
    
    if( t_loginUser.headImg == p_fileInfo.fileId )
    {
        m_loginUser = t_loginUser;
        
        auto t_fileInfo = DataTableFile::instance().find( m_loginUser.headImg );
        if( !t_fileInfo.fileName.empty() )
        {
            m_personalHead->setTexture( t_fileInfo.fileName );
        }
    }
}

