LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := $(LOCAL_PATH)/hellocpp/main.cpp \
                   $(LOCAL_PATH)/Android.cpp \
                   $(LOCAL_PATH)/../../../Classes/AppDelegate.cpp \
                   $(LOCAL_PATH)/../../../Classes/Common/Common.cpp \
                   $(LOCAL_PATH)/../../../Classes/Common/Message.cpp \
                   $(LOCAL_PATH)/../../../Classes/Common/Ajax.cpp \
                   $(LOCAL_PATH)/../../../Classes/Common/WebViewScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/Main/MainScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/Main/DialogPersonalCenterLayer.cpp \
                   $(LOCAL_PATH)/../../../Classes/Main/DialogSettingsLayer.cpp \
                   $(LOCAL_PATH)/../../../Classes/Resources/Login.cpp \
                   $(LOCAL_PATH)/../../../Classes/Resources/Main.cpp \
                   $(LOCAL_PATH)/../../../Classes/Resources/Dialog.cpp \
                   $(LOCAL_PATH)/../../../Classes/DataBase/DataBase.cpp \
                   $(LOCAL_PATH)/../../../Classes/DataBase/DataTableUser.cpp \
                   $(LOCAL_PATH)/../../../Classes/DataBase/sqlite3/sqlite3.c \
                   $(LOCAL_PATH)/../../../Classes/Login/LoginScene.cpp

                   


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes \
                    $(LOCAL_PATH)/../../../Classes/DataBase \
                    $(LOCAL_PATH)/../../../Classes/Login \
                    $(LOCAL_PATH)/../../../Classes/Main \
                    $(LOCAL_PATH)/../../../Classes/Common \
                    $(LOCAL_PATH)/../../../Classes/DataBase/sqlite3 \
                    $(LOCAL_PATH)/../../../Classes/Resources

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, $(LOCAL_PATH)/../../../cocos2d)
$(call import-module, cocos)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
