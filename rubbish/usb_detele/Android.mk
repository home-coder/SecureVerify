LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= usb_detect.c verify.c sha1.c sha1.h hash.h md5_check.c md5_check.h md5.c md5.h verify.h 
LOCAL_MODULE := usb_detect
LOCAL_MODULE_TAGS := optional
LOCAL_FORCE_STATIC_EXECUTABLE := true
LOCAL_STATIC_LIBRARIES := libcutils libc liblog
LOCAL_CFLAGS:= -DLOG_TAG=\"usb_detect\"
include $(BUILD_EXECUTABLE)

