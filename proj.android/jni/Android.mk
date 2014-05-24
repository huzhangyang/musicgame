LOCAL_PATH := $(call my-dir)

#fmod
include $(CLEAR_VARS)

LOCAL_MODULE := fmodex
LOCAL_SRC_FILES := ../../fmod/lib.android/$(TARGET_ARCH_ABI)/libfmodex.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../fmod/inc

include $(PREBUILT_SHARED_LIBRARY)

#cocos2dx
include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared
LOCAL_MODULE_FILENAME := libmusicgame

FILE_LIST := hellocpp/main.cpp
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/*.cpp)
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_network_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

LOCAL_SHARED_LIBRARIES := fmodex
include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)
$(call import-module,Box2D)
$(call import-module,editor-support/cocostudio)
$(call import-module,network)
$(call import-module,extensions)
