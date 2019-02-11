//
//  MainScene.cpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/7/24.
//

#include "MainScene.h"
#include "Common.h"
#include "Main.hpp"
#include "Dialog.hpp"
#include "Config.h"
#include "LoginScene.h"
#include "DataTableFile.h"
#include "Http.h"
#include "DadGuessUpdateScene.h"

#include "WebViewScene.h"
#include "renderer/CCGLProgramStateCache.h"


USING_NS_CC;
using namespace cocos2d::ui;

#define PAGE_FONT "fonts/HuaKangFangYuanTIW7-GB_0.ttf"

Scene * MainScene:: CreateScene( void )
{
    return create();
}

bool MainScene::init( void )
{
    if( !BaseScene::init() )
    {
        return false;
    }
    
    m_loginUser = DataTableUser::instance().getActivation();
    Http::token = m_loginUser.token;
    printf( "%s \n", m_loginUser.toJson().c_str() );
    
    //加载贴图
    TexturePacker::Main::addSpriteFramesToCache();
    TexturePacker::Dialog::addSpriteFramesToCache();
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    float t_PaddingTop = ( visibleSize.height - origin.y ) * 0.15f;
    
    auto t_backgroud = Sprite::create( "BackgroundImage.png" );
    if (t_backgroud != nullptr)
    {
        // position the sprite on the center of the screen
        t_backgroud->setPosition( Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y) );
        auto t_backgroundSize = t_backgroud->getContentSize();
        
        t_backgroud->setScale( visibleSize.width / t_backgroundSize.width, visibleSize.height / t_backgroundSize.height );
        
        this->addChild( t_backgroud, 0 );
    }
    
    auto t_CloundLeft = Sprite::create("Cloud.png");
    auto t_CloundLeftSize = t_CloundLeft->getContentSize();
    if( t_CloundLeft != nullptr )
    {
        t_CloundLeft->setScale( adaptation() );
        t_CloundLeft->setPosition( Vec2(visibleSize.width / 4.0f + origin.x, visibleSize.height + origin.y - t_CloundLeftSize.height * 0.5f - 5.0f - t_PaddingTop ) );
        this->addChild( t_CloundLeft, 0 );
    }
    
    auto t_CloundRight = Sprite::create("Cloud.png");
    if( t_CloundRight != nullptr )
    {
        t_CloundRight->setScale( adaptation() );
        t_CloundRight->setPosition( Vec2(visibleSize.width / 4.0f * 3.0f + origin.x, visibleSize.height + origin.y - t_CloundLeftSize.height * 0.5f - 15.0f - t_PaddingTop) );
        this->addChild( t_CloundRight, 0 );
    }
    
    
    //用户头像

    auto t_userHeadBorder = Button::create( TexturePacker::Main::mainUserHeadBackground, TexturePacker::Main::mainUserHeadBackground, "", Widget::TextureResType::PLIST );
    auto t_userHeadBorderSizeHalf = t_userHeadBorder->getContentSize() * 0.5f;
    auto t_userHeadBorderPosition = Vec2( origin.x + t_userHeadBorderSizeHalf.width * adaptation() + 15.0f , origin.y + visibleSize.height - t_userHeadBorderSizeHalf.height * adaptation() - 10.0f );

    t_userHeadBorder->setPosition( t_userHeadBorderPosition );
    t_userHeadBorder->setScale( adaptation() );
    this->addChild( t_userHeadBorder );
    m_mainSceneButtons.push_back( t_userHeadBorder );


    touchAnswer( t_userHeadBorder , [this]( Ref * p_ref ){
        m_disenableAllButton();
        personalHeadOnClick();
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
    
    auto t_Settings = Button::create( TexturePacker::Main::mainSetting, TexturePacker::Main::mainSetting, "", Widget::TextureResType::PLIST  );
    t_Settings->setScale( adaptation() );
    auto t_SettingsSize = t_Settings->getContentSize();
    t_Settings->setPosition( Vec2( visibleSize.width - t_SettingsSize.width * 2.5f * adaptation() - 45.0f, visibleSize.height + origin.y - t_SettingsSize.height * 0.5f * adaptation() - 20.0f ) );
    this->addChild( t_Settings );
    m_mainSceneButtons.push_back( t_Settings );

    touchAnswer( t_Settings, [this]( Ref * p_ref ){
        m_disenableAllButton();
        settingsOnClick();
    }, adaptation() * 1.1f, adaptation() );

    auto t_Message = Button::create( TexturePacker::Main::mainMessage, TexturePacker::Main::mainMessage, "", Widget::TextureResType::PLIST  );
    t_Message->setScale( adaptation() );
    auto t_MessageSize = t_Message->getContentSize();
    t_Message->setPosition( Vec2( visibleSize.width - t_MessageSize.width * 1.5f * adaptation() - 30.0f, visibleSize.height + origin.y - t_MessageSize.height * 0.5f * adaptation() - 20.0f ) );
    this->addChild( t_Message );
    m_mainSceneButtons.push_back( t_Message );

    touchAnswer( t_Message, [this]( Ref * p_ref ){
        
    }, adaptation() * 1.1f, adaptation() );

    auto t_NewMessage = TexturePacker::Main::createMainNewMessageSprite();
    auto t_NewMessageSize = t_NewMessage->getContentSize();
    t_NewMessage->setPosition( Vec2( t_MessageSize.width - t_NewMessageSize.width * 0.8f, t_MessageSize.height - t_NewMessageSize.height * 0.6f ) );
    t_Message->addChild( t_NewMessage );

    auto t_Notice = Button::create( TexturePacker::Main::mainNotice, TexturePacker::Main::mainNotice, "", Widget::TextureResType::PLIST  );
    t_Notice->setScale( adaptation() );
    auto t_NoticeSize = t_Notice->getContentSize();
    t_Notice->setPosition( Vec2( visibleSize.width - t_NoticeSize.width * 0.5f * adaptation() - 15.0f, visibleSize.height + origin.y - t_NoticeSize.height * 0.5f * adaptation() - 20.0f ) );
    this->addChild( t_Notice );
    m_mainSceneButtons.push_back( t_Notice );
    touchAnswer( t_Notice, [this]( Ref * p_ref ){
        
    }, adaptation() * 1.1f, adaptation() );
    
    Vec2 t_contentPos = Vec2( 30.0f, t_NoticeSize.height * adaptation() + 20.0f );
    Size t_contentSize = Size( visibleSize.width - t_contentPos.x * 2.0f, visibleSize.height - t_contentPos.y - 20  );
    
    Size t_itemSize = Size( t_contentSize.width / 4.0f, t_contentSize.height / 3.0f ) * 0.5f;

    const std::string t_icoList[] = {
        TexturePacker::Main::mainIcoAnimalEncyclopedia,
        TexturePacker::Main::mainIcoRhythmGame,
        TexturePacker::Main::mainIcoDadpat,
        TexturePacker::Main::mainIcoPiano,
        TexturePacker::Main::mainIcoEarth,
        TexturePacker::Main::mainIcoAstronomy,
        TexturePacker::Main::mainIcoChineseHistory,
        TexturePacker::Main::mainIcoHistoryOfTheWorld,
        TexturePacker::Main::mainIcoFamousPainting,
        TexturePacker::Main::mainIcoMonth,
        TexturePacker::Main::mainIcoABC,
        TexturePacker::Main::mainIcoDadGuess
    };
    
    for( int i = 0; i < sizeof(t_icoList) / sizeof(std::string); ++i )
    {
        Vec2 t_pos = Vec2( t_contentPos.x + (i % 4 * 2 + 1) * t_itemSize.width, t_contentPos.y + (i / 4 * 2 + 1) * t_itemSize.height );
        auto t_Ico = Button::create( t_icoList[i], t_icoList[i], "", Widget::TextureResType::PLIST  );
        t_Ico->setScale( adaptation() * 0.7f );
        
        t_Ico->setTag(i);

        auto t_IcoSize = t_Ico->getContentSize();

        t_Ico->setPosition( Vec2( origin.x + t_pos.x , origin.y + visibleSize.height - t_pos.y ) );

        this->addChild( t_Ico );
        
        m_mainSceneButtons.push_back( t_Ico );
        
        touchAnswer( t_Ico, [this]( Ref * p_ref ){
            icoTouchCallBack( ((Button*)p_ref)->getTag() );
                }, adaptation() * 1.0f, adaptation() * 0.7f );
    }

    m_enableAllButton = [this](){
        for( Button * button : m_mainSceneButtons )
        {
            button->setTouchEnabled( true );
        }
    };

    m_disenableAllButton = [this](){
        for( Button * button : m_mainSceneButtons )
        {
            button->setTouchEnabled( false );
        }
    };


    //设置框
    m_dialogSettings = DialogSettingsLayer::create();
    m_dialogSettings->setPosition( Vec2( visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y ) );
    this->addChild( m_dialogSettings, 2 );

    m_dialogSettings->hideCallBack = [this](){
        m_enableAllButton();
    };
    
    //用户中心
    m_dialogPersonalCenter = DialogPersonalCenterLayer::create();
    m_dialogPersonalCenter->setPosition( Vec2( visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y ) );
    this->addChild( m_dialogPersonalCenter );
    
    m_dialogPersonalCenter->hideCallBack = [this, t_personalName](){
        m_enableAllButton();

        auto t_userInfo = DataTableUser::instance().getActivation();

        t_personalName->initWithTTF( t_userInfo.userName, PAGE_FONT, 16 );

        updateUserInfo();

    };

    return true;
}

MainScene::~MainScene()
{
    TexturePacker::Main::removeSpriteFramesFromCache();
    TexturePacker::Dialog::removeSpriteFramesFromCache();
}

void MainScene::personalHeadOnClick( void )
{
    m_dialogPersonalCenter->show();
}

void MainScene::settingsOnClick( void )
{
    m_dialogSettings->show();
}

void MainScene::icoTouchCallBack( int p_icoIndex )
{
    std::string t_dir = "";
    bool t_orientation = false;
    switch (p_icoIndex) {
        case 0:
            t_dir = "Web_Animal";
            t_orientation = true;
            break;
        case 1:
//            Director::getInstance()->replaceScene( CadenceGameLoaderScene::create() );
            return;
        case 2:
            t_dir = "Web_Api";
            break;
        case 3:
//            Director::getInstance()->replaceScene( PianoGameMainScene::CreateScene() );
            return;
        case 4:
            t_dir = "Web_Earth";
            break;
        case 5:
            t_dir = "Web_Astronomy";
            break;
        case 6:
            t_dir = "Web_HistoryChronology";
            break;
        case 7:
            t_dir = "Web_WorldHistory";
            break;
        case 8:
            t_dir = "Web_Paintings";
            break;
        case 9:
            t_dir = "Web_Calendar";
            break;
        case 10:
            t_dir = "Web_ABC";
            break;
        case 11:
            Director::getInstance()->replaceScene( DadGuessUpdateScene::create() );
            return;
        default:
            return;
    }
//    Director::getInstance()->replaceScene( TransitionSlideInR::create( 0.2f, WebViewScene::createWithUrl( t_url, t_orientation ) ) );
    
    Director::getInstance()->replaceScene( WebViewScene::createWithDir( t_dir, t_orientation ) );
}

void MainScene::updateUserInfo( void )
{
    m_loginUser = DataTableUser::instance().getActivation();
}

void MainScene::refreshSource( const DataFileInfo & p_fileInfo )
{
    auto t_loginUser = DataTableUser::instance().getActivation();

    if( t_loginUser.headImg == p_fileInfo.fileId )
    {
        m_loginUser = t_loginUser;

        auto t_fileInfo = DataTableFile::instance().find( m_loginUser.headImg );
        if( !t_fileInfo.fileName.empty() )
        {
//            m_personalHead->setTexture( t_fileInfo.fileName );
            auto director = Director::getInstance();
            Scheduler *sched = director->getScheduler();
            sched->performFunctionInCocosThread( [=](){
                m_personalHead->setTexture( t_fileInfo.fileName );
            } );
        }
    }
}
