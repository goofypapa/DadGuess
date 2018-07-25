//
//  LoginScene.h
//  dadpat-mobile
//
//  Created by 吴思 on 7/3/18.
//

#ifndef __LOGIN_SCENE_H__
#define __LOGIN_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class LoginScene : public cocos2d::Scene
{
public:
    
    enum LoginState{ SelectLogin, PhoneLogin, PhoneRegister, PhoneForgetPassword };
    
    static cocos2d::Scene * CreateScene();

    CREATE_FUNC(LoginScene);

    virtual bool init() override;

    virtual void update( float p_delta ) override;

    void loginWechat( cocos2d::Ref* pSender );
    void loginSina( cocos2d::Ref* pSender );
    void loginPhone( cocos2d::Ref* pSender );
    
    static void loginWechatCallBack( void );

    void loginBack();
    
    void toPhoneRegister( cocos2d::Ref* pSender );
    void toForgetPassword( cocos2d::Ref* pSender );
    
    void login( cocos2d::Ref* pSender );
    void phoneRegister( cocos2d::Ref* pSender );
    void forgetPassword( cocos2d::Ref* pSender  );
    
    void sendVerificationCode( cocos2d::Ref* pSender );
protected:
    virtual ~LoginScene();
private:
    
    cocos2d::Layer * m_SelectLoginType;
    cocos2d::Layer * m_LoginPhone;
    cocos2d::Layer * m_RegisterPhone;
    cocos2d::Layer * m_PhoneForgetPassword;

    cocos2d::Sprite * m_back;
    
    cocos2d::ui::EditBox * m_loginPhoneInput;
    cocos2d::ui::EditBox * m_loginPasswordInput;
    
    cocos2d::ui::EditBox * m_RegisterPhoneInput;
    cocos2d::ui::EditBox * m_RegisterVerificationCodeInput;
    cocos2d::ui::EditBox * m_RegisterPasswordInput;
    
    cocos2d::ui::EditBox * m_ForgetPhoneInput;
    cocos2d::ui::EditBox * m_ForgetVerificationCodeInput;
    cocos2d::ui::EditBox * m_ForgetPasswordInput;
    
    LoginState m_loginState;

    cocos2d::Sprite * m_CloundLeft, * m_CloundRight;
};

#endif /* __LOGIN_SCENE_H__ */
