LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := nativeegl
LOCAL_SRC_FILES := jniapi.cpp rrimagelib.cpp
LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv2

LOCAL_STATIC_LIBRARIES += jpeg
LOCAL_C_INCLUDES += $(LOCAL_PATH)/libjpeg

include $(BUILD_SHARED_LIBRARY)

include $(LOCAL_PATH)/libjpeg/Android.mk