//
//  WebViewScene.hpp
//  dadpat-mobile
//
//  Created by 吴思 on 2018/8/2.
//

#ifndef __WEB_VIEW_SCENE_H__
#define __WEB_VIEW_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <string>

class WebViewScene : public cocos2d::Scene
{
public:
    CREATE_FUNC( WebViewScene );
protected:
    virtual bool init( void ) override;
private:
    cocos2d::experimental::ui::WebView * m_webview;
    std::string m_url;
};

#endif //__WEB_VIEW_SCENE_H__
