LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES := ISharedBuffer.cpp \
                   SharedBufferServer.cpp

LOCAL_SHARED_LIBRARIES:= libcutils libutils libbinder

LOCAL_MODULE := SharedBufferServer

include $(BUILD_EXECUTABLE)

#-----------------------------------------------------------
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES := ISharedBuffer.cpp \
                   SharedBufferClient.cpp

LOCAL_SHARED_LIBRARIES:= libcutils libutils libbinder

LOCAL_MODULE := SharedBufferClient

include $(BUILD_EXECUTABLE)