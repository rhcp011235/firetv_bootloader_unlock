LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
  unlock_firetv.c

LOCAL_MODULE := unlock_firetv
LOCAL_MODULE_TAGS := optional
LOCAL_STATIC_LIBRARIES += libcutils libc
LOCAL_LDFLAGS += -static

TOP_SRCDIR := $(abspath $(LOCAL_PATH))

include $(BUILD_EXECUTABLE)

include $(call all-makefiles-under,$(LOCAL_PATH))
