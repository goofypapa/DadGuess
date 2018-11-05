//
//  PianoGameMusicListLayer.cpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/10/30.
//

#include "PianoGameMusicListLayer.h"
#include "PianoGame.hpp"
#include "Common.h"

USING_NS_CC;
using namespace cocos2d::ui;

bool PianoGameMusicListLayer::init( void )
{
    if( !Layer::init() )
    {
        return false;
    }
    
//    std::string t_musicNameList[] = {
//        "洋娃娃和小熊跳舞",
//        "小兔子",
//        "我是一个粉刷匠",
//        "大长今",
//        "小白船",
//        "伦敦桥",
//        "小花猫拔萝卜",
//        "扬基歌",
//        "玛丽有只小羊羔",
//        "铃儿响叮当"
//    };
    
    m_musicList.push_back( std::pair< std::string, std::string >( "Doll", "洋娃娃和小熊跳舞" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "LittleRabbits", "小兔子" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "PaintingMaster", "我是一个粉刷匠" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "DaeJangGeum", "大长今" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "LittleWhiteBoat", "小白船" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "LondonBridge", "伦敦桥" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "PulloutRadish", "小花猫拔萝卜" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "YankeeDoodle", "扬基歌" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "MaryHadaLittleLamb", "玛丽有只小羊羔" ) );
    m_musicList.push_back( std::pair< std::string, std::string >( "JingleBells", "铃儿响叮当" ) );
    
    PianoGameMusicListScrollView::sm_musicList = m_musicList;
    
    setVisible( false );
    
    auto t_visibleSizeHalf = Director::getInstance()->getVisibleSize() * 0.5f;
    Vec2 t_origin = Director::getInstance()->getVisibleOrigin();
    
    m_contentView = Scale9Sprite::createWithSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( TexturePacker::PianoGame::musicListBackground ) );
    
    float t_topPadding = 10.0f;
    auto t_contentBackgroundSizeHalf = Size( t_visibleSizeHalf.width * 1.5f, t_visibleSizeHalf.height * 1.6f - t_topPadding ) * 0.5f;
    m_contentView->setContentSize( t_contentBackgroundSizeHalf * 2.0f );
    
    m_contentPos = Vec2( 0.0f, -t_topPadding );
    m_contentView->setPosition( m_contentPos + Vec2( 30.0f, 0.0f ) );
    m_contentView->setOpacity( 0 );
    m_contentView->setCascadeOpacityEnabled( true );
    
    addChild( m_contentView );
    
    
    auto t_simpleButtun = Button::create( TexturePacker::PianoGame::musicListButtonActive, TexturePacker::PianoGame::musicListButtonActive, "", Widget::TextureResType::PLIST  );
    t_simpleButtun->setScale( adaptation() );
    auto t_simpleButtunSizeHalf = t_simpleButtun->getContentSize() * t_simpleButtun->getScale() * 0.5f;
    m_buttonFirstPos = Vec2( -t_contentBackgroundSizeHalf.width + t_simpleButtunSizeHalf.width * 0.6f, t_contentBackgroundSizeHalf.height - t_topPadding - t_simpleButtunSizeHalf.height - 15.0f );
    t_simpleButtun->setPosition( m_buttonFirstPos - Vec2( 30.0f, 0.0f ) );
    t_simpleButtun->setCascadeOpacityEnabled( true );
    t_simpleButtun->setOpacity(0);
    addChild( t_simpleButtun );
    
    m_buttonList.push_back( t_simpleButtun );
    
    auto t_simpleLabel = Label::createWithSystemFont( "简单" , "", 16 );
    t_simpleLabel->setScale( 1.0f / t_simpleButtun->getScale() );
    t_simpleLabel->setPosition( Vec2( t_simpleButtunSizeHalf.width, t_simpleButtunSizeHalf.height * 0.9f ) );
    t_simpleButtun->addChild( t_simpleLabel );
    
    auto t_hardButton = Button::create( TexturePacker::PianoGame::musicListButtonNormal, TexturePacker::PianoGame::musicListButtonTouched, "", Widget::TextureResType::PLIST  );
    t_hardButton->setScale( adaptation() );
    auto t_hardButtonPos = Vec2( m_buttonFirstPos.x, m_buttonFirstPos.y - t_simpleButtunSizeHalf.height * 2.0f );
    t_hardButton->setPosition( t_hardButtonPos - Vec2( 30.0f, 0.0f ) );
    t_hardButton->setOpacity( 0 );
    t_hardButton->setCascadeOpacityEnabled( true );
    addChild( t_hardButton );
    
    m_buttonList.push_back( t_hardButton );
    
    auto t_hardLabel = Label::createWithSystemFont( "困难" , "", 16 );
    t_hardLabel->setScale( 1.0f / t_hardLabel->getScale() );
    t_hardLabel->setPosition( Vec2( t_simpleButtunSizeHalf.width, t_simpleButtunSizeHalf.height * 0.9f ) );
    t_hardButton->addChild( t_hardLabel );
    
    auto t_title = Label::createWithSystemFont( "歌单" , "", 20 );
    auto t_titleSizeHalf = t_title->getContentSize() * 0.5f;
    
    t_title->setPosition( Vec2( t_contentBackgroundSizeHalf.width, t_contentBackgroundSizeHalf.height * 2.0f - t_titleSizeHalf.height - 10.0f ) );
    m_contentView->addChild( t_title );
    
    
    auto t_okButton = Button::create( TexturePacker::PianoGame::musicListOkButton, TexturePacker::PianoGame::musicListOkButton, "", Widget::TextureResType::PLIST );
    t_okButton->setScale( adaptation() );
    auto t_okButtonSizeHalf = t_okButton->getContentSize() * t_okButton->getScale() * 0.5f;
    t_okButton->setPosition( Vec2( t_contentBackgroundSizeHalf.width, t_okButtonSizeHalf.height + 5.0f ) );
    m_contentView->addChild( t_okButton );

    
    float t_itemHeight = 20.0f;
    auto t_musicListSizeHalf = Size( t_contentBackgroundSizeHalf.width, (int)( ( t_contentBackgroundSizeHalf.height - t_titleSizeHalf.height -      t_okButtonSizeHalf.height - 10.0f ) / t_itemHeight ) * t_itemHeight );
    m_musicListScrollView = PianoGameMusicListScrollView::create( t_musicListSizeHalf * 2.0f, t_itemHeight );
    
    m_musicListScrollView->setPosition( t_contentBackgroundSizeHalf - t_musicListSizeHalf - Size( 0.0f, t_titleSizeHalf.height - t_okButtonSizeHalf.height ) );
    m_musicListScrollView->setScrollBarColor( Color3B( 255, 255, 255 ) );
    m_contentView->addChild( m_musicListScrollView );
    
    return true;
}

std::pair< std::string, std::string > PianoGameMusicListLayer::getSelectMusic( void )
{
    std::pair< std::string, std::string > t_result;
    
    int t_index = m_musicListScrollView->selectIndex;
    
    std::stringstream t_sstr;
    t_sstr << "PianoGame/MusicScore/" << m_musicList[t_index].first << "/bgm.";
    switch ( t_index ) {
        case 0:
        case 1:
        case 3:
        case 4:
            t_sstr << "mp3";
            break;
        default:
            t_sstr << "wav";
            break;
    }
    t_result.first = t_sstr.str();
    
    t_sstr.str("");
    t_sstr << "PianoGame/MusicScore/" << m_musicList[t_index].first << "/musicScore.json";
    t_result.second = t_sstr.str();
    
    return t_result;
}

void PianoGameMusicListLayer::show( void )
{
    setVisible( true );
    stopAllActions();
    runAction( ActionFloat::create( 0.2f, 0.0f, 1.0f, [this]( const float p_dt ){
        int t_opacity = (int)round( 255.0f * p_dt );
        float t_offset = 30.0f * ( 1.0f - p_dt );
        for( auto t_btn : m_buttonList )
        {
            t_btn->setOpacity( t_opacity );
            t_btn->setPosition( Vec2( m_buttonFirstPos.x + t_offset, t_btn->getPosition().y ) );
        }
        m_contentView->setOpacity( t_opacity );
        m_contentView->setPosition( Vec2( m_contentPos.x - t_offset, m_contentPos.y ) );
    } ) );
}

void PianoGameMusicListLayer::hide( void )
{
    stopAllActions();
    runAction( ActionFloat::create( 0.2f, 1.0f, 0.0f, [this]( const float p_dt ){
        int t_opacity = (int)round( 255.0f * p_dt );
        float t_offset = 30.0f * ( 1.0f - p_dt );
        for( auto t_btn : m_buttonList )
        {
            t_btn->setOpacity( t_opacity );
            t_btn->setPosition( Vec2( m_buttonFirstPos.x - t_offset, t_btn->getPosition().y ) );
        }
        m_contentView->setOpacity( t_opacity );
        m_contentView->setPosition( Vec2( m_contentPos.x + t_offset, m_contentPos.y ) );
        
        if( p_dt == 0.0f )
        {
            setVisible( false );
        }
    } ) );
}
