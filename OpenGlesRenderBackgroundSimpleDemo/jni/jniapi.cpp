#include "jniapi.h"
#include "renderer.h"

JNIEXPORT void JNICALL Java_com_tristan_sun_nativeegl_demo_NativeEglExample_drawBackgroud
  (JNIEnv *env, jclass object, jint width, jint height) {
	start(width, height);
}
