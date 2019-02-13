//
//  LoginScene.h
//  DadGuess-mobile
//
//  Created by 吴思 on 7/3/18.
//

#ifndef __LOGIN_SCENE_H__
#define __LOGIN_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "C2DXShareSDK.h"
#include "DataTableUser.h"
#include "BaseScene.h"
#include "SMSSDK.h"

class LoginScene : public BaseScene, public smssdk::SMSSDKHandler, public cocos2d::ui::EditBoxDelegate
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
    
    static void loginWechatCallBack( const char * p_code );

    void loginBack();
    
    void toPhoneRegister( cocos2d::Ref* pSender );
    void toForgetPassword( cocos2d::Ref* pSender );
    
    void login( cocos2d::Ref* pSender );
    void phoneRegister( cocos2d::Ref* pSender );
    void forgetPassword( cocos2d::Ref* pSender  );
    
    static bool isLogined( void );
    
    static void getUserResultHandler(int reqID, cn::sharesdk::C2DXResponseState state, cn::sharesdk::C2DXPlatType platType, __Dictionary *result);
    
    void registerSendVerificationCode( cocos2d::Ref* pSender );
    void forgetPasswordSendVerificationCode( cocos2d::Ref* pSender );

    virtual void editBoxEditingDidBegin( cocos2d::ui::EditBox *editBox ) override;
    virtual void editBoxEditingDidEnd( cocos2d::ui::EditBox *editBox ) override;
    virtual void editBoxTextChanged( cocos2d::ui::EditBox *editBox,const std::string &text ) override;
    virtual void editBoxReturn( cocos2d::ui::EditBox *editBox ) override;

    void onComplete( smssdk::SMSSDKActionType action,string result ) override ;
    void onError( smssdk::SMSSDKActionType action,string result ) override ;

protected:
    virtual ~LoginScene();
private:
    
    static void loginCallBack( const std::string & p_str );
    
    static bool m_requesting;


    std::string m_verificationCode, m_verificationCodePhone;

    bool m_verificationCodeVerification;
    int m_registerSendVerificationCodeCoolDown, m_forgetPasswordSsendVerificationCodeCoolDown;

    cocos2d::MenuItemFont * m_registerSendVerificationCode, * m_forgetPasswordSsendVerificationCode;
    
    cocos2d::Layer * m_SelectLoginType;
    cocos2d::Layer * m_LoginPhone;
    cocos2d::Layer * m_RegisterPhone;
    cocos2d::Layer * m_PhoneForgetPassword;

    cocos2d::ui::Button * m_back;
    
    cocos2d::ui::EditBox * m_loginPhoneInput;
    cocos2d::ui::EditBox * m_loginPasswordInput;
    
    cocos2d::ui::EditBox * m_RegisterPhoneInput;
    cocos2d::ui::EditBox * m_RegisterVerificationCodeInput;
    cocos2d::ui::EditBox * m_RegisterPasswordInput;
    
    cocos2d::ui::EditBox * m_ForgetPhoneInput;
    cocos2d::ui::EditBox * m_ForgetVerificationCodeInput;
    cocos2d::ui::EditBox * m_ForgetPasswordInput;
    
    LoginState m_loginState;
    
    static DataUserInfo::LoginType m_loginType;

    cocos2d::Sprite * m_CloundLeft, * m_CloundRight;
};

#endif /* __LOGIN_SCENE_H__ */
