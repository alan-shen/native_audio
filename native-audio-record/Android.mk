
LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE:= native_audio_record

LOCAL_SRC_FILES := \
    native-audio-record.cpp

LOCAL_SHARED_LIBRARIES :=  \
    libc \
    liblog \
    libcutils \
    libutils \
    libbinder \
    libhardware_legacy \
    libmedia \
    libaudioclient \

LOCAL_MODULE_TAGS := tests

LOCAL_CFLAGS += -Wall -Werror -Wno-error=deprecated-declarations -Wunused -Wunreachable-code

include $(BUILD_EXECUTABLE)
