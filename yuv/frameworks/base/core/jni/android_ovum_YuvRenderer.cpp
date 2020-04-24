//
// Created by illusion on 17-12-7.
//
#define LOG_TAG "YuvRendererJNI"
#include <jni.h>
#include <android_runtime/AndroidRuntime.h>
#include <android_runtime/android_view_Surface.h>
#include <gui/Surface.h>
#include <assert.h>
#include <utils/Log.h>
#include <JNIHelp.h>
#include <media/stagefright/foundation/ADebug.h>
#include <ui/GraphicBufferMapper.h>
#include <cutils/properties.h>

using namespace android;

namespace android {

static sp<ANativeWindow> mNativeWindow = NULL;
static int mCropWidth;
static int mCropHeight;

static int ALIGN(int x, int y) {
    // y must be a power of 2.
    return (x + y - 1) & ~(y - 1);
}

static void render(const void *data, int size) {
    int err;

    ANativeWindowBuffer *buf;//描述buffer
    int fenceFd = -1;
    err = mNativeWindow->dequeueBuffer(mNativeWindow.get(), &buf, &fenceFd);
    if (err != 0) {
        ALOGW("dequeue buffer returned error %d", err);
        return;
    }

    GraphicBufferMapper &mapper = GraphicBufferMapper::get();

    Rect bounds(mCropWidth, mCropHeight);

    void *dst;
    CHECK_EQ(0, mapper.lock(//用来锁定一个图形缓冲区并将缓冲区映射到用户进程
        buf->handle, GRALLOC_USAGE_SW_WRITE_OFTEN, bounds, &dst));//dst就指向图形缓冲区首地址

    if (true){
        size_t dst_y_size = buf->stride * buf->height;
        size_t dst_c_stride = ALIGN(buf->stride / 2, 16);//1行v/u的大小
        size_t dst_c_size = dst_c_stride * buf->height / 2;//u/v的大小

        memcpy(dst, data, dst_y_size + dst_c_size*2);//将yuv数据copy到图形缓冲区
    }

    CHECK_EQ(0, mapper.unlock(buf->handle));

    if ((err = mNativeWindow->queueBuffer(mNativeWindow.get(), buf, -1)) != 0) {
        ALOGW("queueBuffer returned error %d", err);
    }
    buf = NULL;
}

static void init(sp<Surface> surface, int width, int height) {
    mNativeWindow = surface;
    mCropWidth = width;
    mCropHeight = height;

    int halFormat = HAL_PIXEL_FORMAT_YV12;//颜色空间
    int bufWidth = (mCropWidth + 1) & ~1;//按2对齐
    int bufHeight = (mCropHeight + 1) & ~1;

    CHECK_EQ(0, native_window_set_usage(
                    mNativeWindow.get(),
                    GRALLOC_USAGE_SW_READ_NEVER | GRALLOC_USAGE_SW_WRITE_OFTEN
                    | GRALLOC_USAGE_HW_TEXTURE | GRALLOC_USAGE_EXTERNAL_DISP));

    CHECK_EQ(0, native_window_set_scaling_mode(
                    mNativeWindow.get(),
                    NATIVE_WINDOW_SCALING_MODE_SCALE_TO_WINDOW));

    // Width must be multiple of 32???
    CHECK_EQ(0, native_window_set_buffers_dimensions(
                mNativeWindow.get(),
                bufWidth,
                bufHeight));
    CHECK_EQ(0, native_window_set_buffers_format(
                mNativeWindow.get(),
                halFormat));
}

static void android_ovum_YuvRenderer_setSurface(JNIEnv* env, jobject /* this */, jint width, jint height, jobject jsurface) {
    ALOGI("setSurface");
    sp<Surface> surface = android_view_Surface_getSurface(env, jsurface);
    if(android::Surface::isValid(surface)){
        ALOGE("surface is valid");
    } else {
        ALOGE("surface is invalid");
        mNativeWindow = NULL;
        return;
    }

    init(surface, width, height);
}

static void android_ovum_YuvRenderer_render(JNIEnv* env, jobject /* this */, jbyteArray arrayBuffer, jint size) {
    if (mNativeWindow == NULL) {
        ALOGE("mNativeWindow is NULL, return");
    }
    void* data = (void*)env->GetByteArrayElements(arrayBuffer, 0);
    render(data, size);
    env->ReleaseByteArrayElements(arrayBuffer, (jbyte*)data, 0);
}

static const JNINativeMethod sMethods[] = {
    { "setSurface", "(IILandroid/view/Surface;)V", (void *)android_ovum_YuvRenderer_setSurface},
    { "render", "([BI)V", (void *)android_ovum_YuvRenderer_render},
};

int register_android_ovum_YuvRenderer(JNIEnv* env) {
    return jniRegisterNativeMethods(env, "android/ovum/YuvRenderer", sMethods, NELEM(sMethods));
}

} // namespace android