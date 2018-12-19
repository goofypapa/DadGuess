#include "DadGuessBlueScene.h"
#include "Common.h"
#include "DadGuessMainScene.h"

#include "DadGuessMain.hpp"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

//android 
bool DadGuessBlueScene::init( void )
{
    if( !BaseScene::init() )
    {
        return false;
    }

    auto t_visibleSizeHalf = Director::getInstance()->getVisibleSize() * 0.5f;
    Vec2 t_origin = Director::getInstance()->getVisibleOrigin();

    auto t_sceneCenter = t_origin + t_visibleSizeHalf;

    auto t_background = DrawNode::create();
    t_background->drawSolidRect( Vec2::ZERO , t_visibleSizeHalf * 2.0f, createColor4FWithStr( "7ad4f9" ) );

    t_background->setPosition( t_origin );
    
    addChild( t_background );

    auto t_btnBack = Button::create( TexturePacker::DadGuessMain::caicai_back, TexturePacker::DadGuessMain::caicai_back, "", Button::TextureResType::PLIST );
    t_btnBack->setScale( adaptation() );
    auto t_btnBackSizeHalf = t_btnBack->getContentSize() * t_btnBack->getScale() * 0.5f;
    t_btnBack->setPosition( Vec2( t_origin.x + t_btnBackSizeHalf.width + 15.0f, t_origin.y + t_visibleSizeHalf.height * 2.0f - t_btnBackSizeHalf.height - 15.0f ) );
    addChild( t_btnBack );
    
    touchAnswer( t_btnBack , []( Ref * p_target ){
        Director::getInstance()->replaceScene( DadGuessMainScene::create() );
        BlueDeviceListener::cancelScan();
    }, adaptation() * 1.2f, adaptation() );

    auto t_blueTips = Label::createWithSystemFont( "确保硬件处于开机状态，并且距离手机5米范围内", "", 14 );
    auto t_blueTipsSizeHalf = t_blueTips->getContentSize() * 0.5f;
    t_blueTips->setPosition( Vec2( t_sceneCenter.x, t_origin.y + t_blueTipsSizeHalf.height + 10.0f ) );
    addChild( t_blueTips );

    t_sceneCenter.y += t_blueTipsSizeHalf.height;

    //搜索动画

    std::list< Sprite * > t_blueSechBgList;

    for( int i = 0; i < 3; ++i )
    {
        auto t_blueSechBg = TexturePacker::DadGuessMain::createCaicai_bluesearch_pic_backgroundSprite();
        auto t_blueSechBgSizeHalf = t_blueSechBg->getContentSize() * 0.5f;

        t_blueSechBg->setScale( ( t_visibleSizeHalf.height * 0.4f ) / t_blueSechBgSizeHalf.height  * ( 1.0f + i * 0.4f ) );
        t_blueSechBg->setPosition( t_sceneCenter );
        t_blueSechBg->setOpacity( 200 - i * 50 );
        addChild( t_blueSechBg, 10 - i * -1 );
    }

    
    auto t_blueIcon = TexturePacker::DadGuessMain::createCaicai_bluesearch_pic_blueSprite();
    t_blueIcon->setScale( adaptation() );
    t_blueIcon->setPosition( t_sceneCenter );
    addChild( t_blueIcon, 100 );

    m_blueDeviceListener = new BlueDeviceListener(  [this]( bool p_connected ){
        printf( "------------->connected: %s \n", ( p_connected ? "true" : "false" ) );

        if( p_connected )
        {
            Director::getInstance()->replaceScene( DadGuessMainScene::create() );
        }else{
            printf( "-------connect fial\n" );
        }

    }, nullptr, [this, t_sceneCenter]( const std::string & p_address, const std::string & p_deviceName ){
        printf( "---------> scan device: %s-%s \n", p_address.c_str(), p_deviceName.c_str() );
        
        for( auto t_deviceAddress : m_blueDeviceList )
        {
            if( t_deviceAddress.compare( p_address ) == 0 )
            {
                return;
            }
        }

        //创建按钮
        auto t_connectBtn = Button::create( TexturePacker::DadGuessMain::caicai_bluesearch_pic_fontbj, TexturePacker::DadGuessMain::caicai_bluesearch_pic_fontbj, "", Button::TextureResType::PLIST );
        auto t_connectBtnSizeHalf = t_connectBtn->getContentSize() * 0.5f;

        float t_arge = rand_0_1() * 2.0f * PI;

        t_connectBtn->setPosition( t_sceneCenter + Vec2( cosf( t_arge ) * t_connectBtnSizeHalf.width, sinf( t_arge ) * t_connectBtnSizeHalf.width ) * 1.8f );
        t_connectBtn->setScale( adaptation() );

        addChild( t_connectBtn, 101 );

        auto t_blueName = Label::createWithSystemFont( p_deviceName, "", 12 );
        t_blueName->setPosition( Vec2( t_connectBtnSizeHalf.width, t_connectBtnSizeHalf.height) );
        t_blueName->setColor( Color3B( createColor4FWithStr( "35a1ec" ) ) );
        t_connectBtn->addChild( t_blueName );

        t_connectBtn->setTag( m_blueDeviceList.size() );
        touchAnswer( t_connectBtn , [this]( Ref * p_target ){
            BlueDeviceListener::cancelScan();
            BlueDeviceListener::connect( m_blueDeviceList[ ( (Button *)p_target )->getTag() ] );
        }, adaptation() * 1.2f, adaptation() );

        m_blueDeviceList.push_back( p_address );

    } );


    BlueDeviceListener::scan();

    return true;
}

 DadGuessBlueScene::~DadGuessBlueScene( void )
 {
     delete m_blueDeviceListener;
     
 }