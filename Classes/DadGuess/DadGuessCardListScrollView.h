//
//  DadGuessCardListScrollView.h
//  DadGuess-mobile
//
//  Created by 吴思 on 2018/11/22.
//

#ifndef __DAD_GUESS_CARD_LIST_SCROLL_VIEW_H__
#define __DAD_GUESS_CARD_LIST_SCROLL_VIEW_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "DataTableCard.h"
#include "DataTableFile.h"
#include "Http.h"
#include <map>

#include <vector>
#include <queue>
#include <functional>

class DadGuessCardListScrollView : public cocos2d::ui::ScrollView
{
public:
    static DadGuessCardListScrollView * createWithSize( const cocos2d::Size p_size, const std::string p_groupId );
protected:
    CREATE_FUNC( DadGuessCardListScrollView );
    
    virtual bool init( void ) override;
    
    virtual bool initWithSize( const cocos2d::Size & p_contentSize, const std::string p_groupId );
    
    void onTouched( const int p_index );
    
    virtual bool onTouchBegan( cocos2d::Touch * p_touch, cocos2d::Event * p_unusedEvent) override;
    virtual void onTouchMoved( cocos2d::Touch * p_touch, cocos2d::Event * p_unusedEvent) override;
    virtual void onTouchEnded( cocos2d::Touch * p_touch, cocos2d::Event * p_unusedEvent) override;
//    virtual void onTouchCancelled( cocos2d::Touch * p_touch, cocos2d::Event * p_unusedEvent) override;
    
    void loadImage( void );
    
    virtual ~DadGuessCardListScrollView();
private:
    static int sm_columns;
    std::string m_groupId;
    
    static std::list< Http * > sm_invalidDownloadList;
    static std::list< Http * > sm_downloadingList;
    
    std::map< int, cocos2d::Vec2 > m_touchBeginLocationList;
    
    std::queue< std::pair< std::string, std::function< void( DataFileInfo ) > > > m_downloadPool;
    
};

#endif //__DAD_GUESS_CARD_LIST_SCROLL_VIEW_H__
