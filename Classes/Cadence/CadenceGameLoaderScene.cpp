//
//  CadenceGameLoaderScene.cpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/11/6.
//

#include "cocos2d.h"
#include "CadenceGameLoaderScene.h"
#include "CadenceGameMainScene.h"

#include "AudioEngine.h"

USING_NS_CC;
using namespace experimental;

const char * CadenceGameLoaderScene::sm_loadImgList[] = {
    "Cadence/CadenceMain.png"
};

const char * CadenceGameLoaderScene::sm_loadAudioList[] = {
    "Cadence/Djembe/B.wav",
    "Cadence/Djembe/T.wav",
    "Cadence/Djembe/S.wav"
};



bool CadenceGameLoaderScene::init( void )
{
    if( !BaseScene::init() )
    {
        return false;
    }
    
    cacheResource();
    
    return true;
}

void CadenceGameLoaderScene::cacheResource( void )
{
    size_t t_loadImgListSize = sizeof( sm_loadImgList ) / sizeof( char * );
    size_t t_loadAudioListSize = sizeof( sm_loadAudioList ) / sizeof( char * );
    
    m_loadResourceCount = t_loadImgListSize + t_loadAudioListSize;
    
    auto tf_tryStartGame = [this](){
        if( --m_loadResourceCount == 0 )
        {
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile( "Cadence/CadenceMain.plist", Director::getInstance()->getTextureCache()->getTextureForKey( "Cadence/CadenceMain.png" ) );
            
            Director::getInstance()->replaceScene( CadenceGameMainScene::create() );
        }
    };
    
    for( size_t i = 0; i < t_loadImgListSize; ++i )
    {
        Director::getInstance()->getTextureCache()->addImageAsync( sm_loadImgList[i], [=]( Texture2D * p_txt ){
            tf_tryStartGame();
        });
    }
    
    for( size_t i = 0; i < t_loadAudioListSize; ++i )
    {
        AudioEngine::preload( sm_loadAudioList[i], [=]( bool p_res ){
            tf_tryStartGame();
        });
    }
}

void CadenceGameLoaderScene::uncacheResource( void )
{
    size_t t_loadImgListSize = sizeof( sm_loadImgList ) / sizeof( char * );
    size_t t_loadAudioListSize = sizeof( sm_loadAudioList ) / sizeof( char * );
    
    SpriteFrameCache::getInstance()->removeSpriteFramesFromTexture( Director::getInstance()->getTextureCache()->getTextureForKey( "Cadence/CadenceMain.png" ) );
    
    for( size_t i = 0; i < t_loadImgListSize; ++i )
    {
        if( Director::getInstance()->getTextureCache()->getTextureForKey( sm_loadImgList[i] ) != nullptr )
        {
            Director::getInstance()->getTextureCache()->removeTextureForKey( sm_loadImgList[i] );
        }
    }
    
    for( size_t i = 0; i < t_loadAudioListSize; ++i )
    {
        AudioEngine::uncache( sm_loadAudioList[i] );
    }
}

CadenceGameLoaderScene::~CadenceGameLoaderScene()
{
    
}
