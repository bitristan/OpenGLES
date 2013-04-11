#include <stdint.h>
#include <unistd.h>
#include <pthread.h>

#include "renderer.h"
#include "rrimagelib.h"

void start(int width, int height) {
	const EGLint attribs[] = { EGL_SURFACE_TYPE, EGL_PBUFFER_BIT, EGL_BLUE_SIZE,
			8, EGL_GREEN_SIZE, 8, EGL_RED_SIZE, 8, EGL_NONE };
	const EGLint context_attribs[] = { EGL_WIDTH, width, EGL_HEIGHT, height,
			EGL_NONE };

	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;

	EGLConfig config;
	EGLint numConfigs;
	EGLint format;

	if ((display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY) {
		LOGD("eglGetDisplay() returned error %d", eglGetError());
		return;
	}
	if (!eglInitialize(display, 0, 0)) {
		LOGD("eglInitialize() returned error %d", eglGetError());
		return;
	}

	if (!eglChooseConfig(display, attribs, &config, 1, &numConfigs)) {
		LOGD("eglChooseConfig() returned error %d", eglGetError());
		eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		eglDestroyContext(display, context);
		eglDestroySurface(display, surface);
		eglTerminate(display);

		display = EGL_NO_DISPLAY;
		surface = EGL_NO_SURFACE;
		context = EGL_NO_CONTEXT;
		return;
	}

	if (!eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format)) {
		LOGD("eglGetConfigAttrib() returned error %d", eglGetError());
		eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		eglDestroyContext(display, context);
		eglDestroySurface(display, surface);
		eglTerminate(display);

		display = EGL_NO_DISPLAY;
		surface = EGL_NO_SURFACE;
		context = EGL_NO_CONTEXT;
		return;
	}

	if (!(context = eglCreateContext(display, config, 0, 0))) {
		LOGD("eglCreateContext() returned error %d", eglGetError());
		eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		eglDestroyContext(display, context);
		eglDestroySurface(display, surface);
		eglTerminate(display);

		display = EGL_NO_DISPLAY;
		surface = EGL_NO_SURFACE;
		context = EGL_NO_CONTEXT;
		return;
	}

	if (!(surface = eglCreatePbufferSurface(display, config, context_attribs))) {
		LOGD("eglCreatePbufferSurface() returned error %d", eglGetError());
		eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		eglDestroyContext(display, context);
		eglDestroySurface(display, surface);
		eglTerminate(display);

		display = EGL_NO_DISPLAY;
		surface = EGL_NO_SURFACE;
		context = EGL_NO_CONTEXT;
		return;
	}

	if (!eglMakeCurrent(display, surface, surface, context)) {
		LOGD("eglMakeCurrent() returned error %d", eglGetError());
		eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		eglDestroyContext(display, context);
		eglDestroySurface(display, surface);
		eglTerminate(display);

		display = EGL_NO_DISPLAY;
		surface = EGL_NO_SURFACE;
		context = EGL_NO_CONTEXT;
		return;
	}

	glClearColor(0, 0, 1, 1);
	glViewport(0, 0, width, height);

	if (display) {
		glClear (GL_COLOR_BUFFER_BIT);
	}

	unsigned char *pixels = (unsigned char *) malloc(width * height * 4);
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	rrimage *data = init_rrimage();
	data->width = width;
	data->height = height;
	data->channels = 4;
	data->pixels = pixels;
	strip_alpha(data);

	LOGD("write result = %d\n", write_jpeg("/sdcard/test.jpg", data));
	free_rrimage(data);

	return;
}
