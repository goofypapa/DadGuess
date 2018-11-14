//
//  DadGuessUpdateScene.cpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/10/29.
//

#include "DadGuessUpdateScene.h"
#include "DadGuess.hpp"
#include "Common.h"
#include "Http.h"

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

USING_NS_CC;
using namespace rapidjson;

#define DOMAIN_NAME "http://www.dadpat.com/"

const char * DadGuessUpdateScene::sm_batchListApi = DOMAIN_NAME "resource/batch/list/summary.do";

cocos2d::Scene * DadGuessUpdateScene::CreateScene( void )
{
    return create();
}

bool DadGuessUpdateScene::init( void )
{
    if( !BaseScene::init() )
    {
        return false;
    }
    
    TexturePacker::DadGuess::addSpriteFramesToCache();
    
    auto t_visibleSizeHalf = Director::getInstance()->getVisibleSize() * 0.5f;
    Vec2 t_origin = Director::getInstance()->getVisibleOrigin();
    
    auto t_centerPos = Vec2( t_origin.x + t_visibleSizeHalf.width, t_origin.y + t_visibleSizeHalf.height );
    
    DrawNode * t_backgroundColor = DrawNode::create();
    
    t_backgroundColor->setPosition( Vec2( t_origin.x + t_visibleSizeHalf.width, t_origin.y + t_visibleSizeHalf.height ) );
    
    t_backgroundColor->drawSolidRect( Vec2( -t_visibleSizeHalf.width, -t_visibleSizeHalf.height ), Vec2( t_visibleSizeHalf.width, t_visibleSizeHalf.height ), Color4F( 122.0f / 255.0f, 212.0f / 255.0f, 249.0f / 255.0f, 1.0f ) );
    
    addChild( t_backgroundColor );
    
    auto t_head = TexturePacker::DadGuess::createHeadSprite();
    t_head->setScale( adaptation() );
    auto t_headSizeHalf = t_head->getContentSize() * 0.5f;
    auto t_headPos = t_centerPos + Vec2( 0.0f, t_visibleSizeHalf.height * 0.2f );
    t_head->setPosition( t_headPos );
    addChild( t_head );
    
    auto t_dot = TexturePacker::DadGuess::createDotSprite();
    float t_dotDistance = t_headSizeHalf.width * 1.5f;
    t_dot->setPosition( Vec2( t_headSizeHalf.width, t_headSizeHalf.height ) );
    t_head->addChild( t_dot );
    
    auto  t_eyeDistance = t_headSizeHalf.width * 0.07f;
    auto t_eyeLeft = TexturePacker::DadGuess::createEyeSprite();
    auto t_eyeLeftPos = Vec2( t_headSizeHalf.width * 0.73f, t_headSizeHalf.height * 0.885f );
    t_eyeLeft->setPosition( t_eyeLeftPos );
    t_head->addChild( t_eyeLeft );
    
    auto t_eyeRight = TexturePacker::DadGuess::createEyeSprite();
    auto t_eyeRightPos = Vec2( t_headSizeHalf.width * 1.27f, t_headSizeHalf.height * 0.885f );
    t_eyeRight->setPosition( t_eyeRightPos );
    t_head->addChild( t_eyeRight );
    
    
    t_head->runAction( RepeatForever::create( ActionFloat::create( 3.0f , 0.0f, 1.0f, [=]( const float t_dt ){
        float t_seed = t_dt * PI * -2.0f;
        t_dot->setPosition( Vec2( t_headSizeHalf.width + cos( t_seed ) * t_dotDistance, t_headSizeHalf.height + sin( t_seed ) * t_dotDistance ) );
        t_dot->setRotation( t_dt * 360.0f );
        
        auto t_eyeOffset = Vec2( cos( t_seed ) * t_eyeDistance, sin( t_seed ) * t_eyeDistance );
        t_eyeLeft->setPosition( t_eyeLeftPos + t_eyeOffset );
        t_eyeRight->setPosition( t_eyeRightPos + t_eyeOffset );
        
    }) ) );
    
    
    //更新内容提示
    auto t_message = Label::createWithSystemFont( "正在检查更新..." , "", 12 );
    t_message->setPosition( t_headPos - Vec2( 0.0f, t_dotDistance * adaptation() * 2.0f ) );
    addChild( t_message );
    
    
    //检查更新
    
    
//    const char * p = "--------->" DOMAIN_NAME;
//    printf( "%s \n", p );
//
    Http::HttpParameter t_parameter;
    
    Http::Post( sm_batchListApi , &t_parameter, []( Http * p_http, std::string p_res ){
        printf( "------> %s \n", p_res.c_str() );
        
        Document t_readdoc;
        
        t_readdoc.Parse<0>( p_res.c_str() );
        
        if( t_readdoc.HasParseError() )
        {
            printf( "GetParseError %d \n", t_readdoc.GetParseError() );
        }
        
    }, []( Http * p_http, std::string p_res ){
        
    });
    
    
    return true;
}

DadGuessUpdateScene::~DadGuessUpdateScene( void )
{
    TexturePacker::DadGuess::removeSpriteFramesFromCache();
}
