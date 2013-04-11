#ifndef RENDERER_H
#define RENDERER_H

#include <pthread.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h> // requires ndk r5 or newer

void start(int width, int height);

#endif // RENDERER_H
