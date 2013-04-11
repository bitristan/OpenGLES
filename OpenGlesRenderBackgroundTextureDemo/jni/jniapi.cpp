#include "jniapi.h"
#include "rrimagelib.h"

#include <GLES2/gl2.h>
#include <EGL/egl.h>

GLuint loadShader(const char *shaderSrc, GLenum type) {
	GLuint shader;
	GLint compiled;

	shader = glCreateShader(type);
	if (shader == 0) {
		return 0;
	}

	glShaderSource(shader, 1, &shaderSrc, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		GLint infoLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen > 1) {
			char* infoLog = (char *) malloc(sizeof(char) * infoLen);
			glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
			LOGD("Error compiling shader:\n%s\n", infoLog);
			free(infoLog);
		}
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

JNIEXPORT void JNICALL Java_com_tristan_sun_nativeegl_demo_NativeEglExample_drawBackgroud
(JNIEnv *env, jclass object, jint width, jint height) {
	const EGLint configAttribs[] = {EGL_SURFACE_TYPE, EGL_PBUFFER_BIT, EGL_BLUE_SIZE,
		8, EGL_GREEN_SIZE, 8, EGL_RED_SIZE, 8, EGL_NONE};
	const EGLint surfaceAttribs[] = {EGL_WIDTH, width, EGL_HEIGHT, height,
		EGL_NONE};
	const EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE};

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
	if (!eglChooseConfig(display, configAttribs, &config, 1, &numConfigs)) {
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

	if (!(context = eglCreateContext(display, config, 0, contextAttribs))) {
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

	if (!(surface = eglCreatePbufferSurface(display, config, surfaceAttribs))) {
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

	glClearColor(0, 1, 1, 1);
	glViewport(0, 0, width, height);

	const char vShaderStr[] =
	"attribute vec4 vPosition; attribute vec4 inputTextureCoordinate; varying vec2 textureCoordinate; \
	void main() { gl_Position = vPosition; textureCoordinate = inputTextureCoordinate.xy; }";
	const char fShaderStr[] =
	"precision mediump float; varying highp vec2 textureCoordinate; uniform sampler2D inputImageTexture; \
	void main() { gl_FragColor = texture2D(inputImageTexture, textureCoordinate); }";

	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameterf(GL_TEXTURE_2D,
			GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
			GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			GL_CLAMP_TO_EDGE);

	rrimage *path = read_jpeg("/sdcard/path.jpg");
	if (path) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, path->width, path->height, 0, GL_RGB, GL_UNSIGNED_BYTE, path->pixels);
	}

	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint programObject;
	GLint linked;
	vertexShader = loadShader(vShaderStr, GL_VERTEX_SHADER);
	fragmentShader = loadShader(fShaderStr, GL_FRAGMENT_SHADER);

	programObject = glCreateProgram();
	if (programObject == 0) {
		return;
	}
	glAttachShader(programObject, vertexShader);
	glAttachShader(programObject, fragmentShader);
	glLinkProgram(programObject);
	glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
	if (!linked) {
		GLint infoLen = 0;
		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen > 1) {
			char* infoLog = (char *) malloc(sizeof(char) * infoLen);
			glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
			LOGD("Error linking program:\n%s\n", infoLog);
			free(infoLog);
		}
		glDeleteProgram(programObject);
		return;
	}

	int positionHandler = glGetAttribLocation(programObject, "vPosition");
	int coordinateHandler = glGetAttribLocation(programObject, "inputTextureCoordinate");
	int textureHandler = glGetUniformLocation(programObject, "inputImageTexture");

	glClear (GL_COLOR_BUFFER_BIT);
	glUseProgram(programObject);

	GLfloat vVertices[] = {-1.0f, 1.0f, 0.0f, -1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f};
	//glReadPixels读出像素是上下翻转的，于是这里做一个上下镜像翻转，避免对读出数据再处理
	//GLfloat vCoordinates[] = {0.0f, 0.0f, 0.0f,
	//1.0f, 1.0f, 0.0f, 1.0f, 1.0f,};
	GLfloat vCoordinates[] = {0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 1.0f, 1.0f, 0.0f,};

	glVertexAttribPointer(positionHandler, 3, GL_FLOAT,
			GL_FALSE, 0, vVertices);
	glEnableVertexAttribArray(positionHandler);

	glVertexAttribPointer(coordinateHandler, 2, GL_FLOAT,
			GL_FALSE, 0, vCoordinates);
	glEnableVertexAttribArray(coordinateHandler);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glUniform1i(textureHandler, 0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableVertexAttribArray(positionHandler);
	glDisableVertexAttribArray(coordinateHandler);
	glBindTexture(GL_TEXTURE_2D, 0);

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

