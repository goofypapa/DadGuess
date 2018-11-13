#include "CadenceGameMainScene.h"
#include "CadenceMain.hpp"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Common.h"
#include "MainScene.h"
#include "CadenceGameLoaderScene.h"
#include "CadenceGameMusicListScrollView.h"
#include "CadenceGameScene.h"

USING_NS_CC;
using namespace cocos2d::ui;

bool CadenceGameMainScene::init( void )
{
    if( !BaseScene::init() )
    {
        return false;
    }
    
    
    m_musicList.push_back( std::pair< std::string, std::string >( "ThreeLittleBears", "三只小熊" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "KissMyBaby", "亲亲我的宝贝" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "LondonBridge", "伦敦桥" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "Hello", "你好" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "LovelySmurf", "可爱的蓝精灵" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "TheKingMakesMePetrolInTheMountain", "大王叫我来巡山" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "DaeJangGeum", "大长今" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "GoodLuck", "好运来" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "SmallRabbitLambkin", "小兔子乖乖" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "LittleWhiteBoat", "小白船" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "SmallFlowerCatPullRadish", "小花猫拔萝卜" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "BabyCrocodile", "小鳄鱼儿童版" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "ShaolinHero", "少林英雄" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "IWon'tBeFooledByYou", "我不上你的当" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "IAmAPainter", "我是一个粉刷匠" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "IAmALittleGreenDragon", "我是一条小青龙" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "YankeeDoodle", "扬基歌" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "Dolly", "洋娃娃" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "BathingSong", "洗澡歌" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "SeaweedDance", "海草舞" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "MaryHadALittleLamb", "玛丽有只小羔羊" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "GoodAndBeautiful", "真善美" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "RedDragonfly", "红蜻蜓" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "Cucurbit", "葫芦娃" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "TreadSnowToSeekPlum", "踏雪寻梅" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "Ponyo", "金鱼姬" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "JingleBells", "铃儿响叮当" ) );
    

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto t_backgroud = Sprite::create( "Cadence/CadenceBackground_1.png" );
    if (t_backgroud != nullptr)
    {
        t_backgroud->setPosition( Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y) );
        auto t_backgroundSize = t_backgroud->getContentSize();
        
        t_backgroud->setScale( visibleSize.width / t_backgroundSize.width, visibleSize.height / t_backgroundSize.height );
        
        this->addChild( t_backgroud, 0 );
    }

    auto t_back = Button::create( "Back.png" );
    auto t_backSizeHalf = t_back->getContentSize() * 0.5f;
    t_back->setScale( adaptation() );
    t_back->setPosition( Vec2( origin.x + t_backSizeHalf.width + 20.0f, origin.y + visibleSize.height - t_backSizeHalf.height - 20.0f ) );

    this->addChild( t_back );

    touchAnswer( t_back, []( Ref * p_ref ){
        Director::getInstance()->replaceScene( MainScene::create() );
        CadenceGameLoaderScene::uncacheResource();
    }, adaptation() * 1.1f, adaptation() );
    
    
    auto t_musicListScrollView = CadenceGameMusicListScrollView::create();
    auto t_musicListScrollViewSize = Size( visibleSize.width * 0.8f, visibleSize.height);
    t_musicListScrollView->setContentSize( t_musicListScrollViewSize );
    t_musicListScrollView->setPosition( Vec2( origin.x + ( visibleSize.width - t_musicListScrollViewSize.width ) * 0.5f, origin.y ) );
    t_musicListScrollView->setInnerContainerSize( Size( t_musicListScrollViewSize.width, m_musicList.size() * 30.0f ) );
    t_musicListScrollView->setDirection( ScrollView::Direction::VERTICAL );
    
    addChild( t_musicListScrollView );
    
    t_musicListScrollView->initWithData( m_musicList );
    
    t_musicListScrollView->onTouched = [this]( const int p_index ){
      
        Director::getInstance()->replaceScene( CadenceGameScene::CreateScene( m_musicList[p_index].first, m_musicList[p_index].second ) );
    };
    
    return true;
}

CadenceGameMainScene::~CadenceGameMainScene()
{
}
