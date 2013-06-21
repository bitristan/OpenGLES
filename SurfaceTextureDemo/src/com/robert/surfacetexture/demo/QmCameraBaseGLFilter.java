package com.robert.surfacetexture.demo;

import java.util.HashMap;

import android.graphics.Bitmap;
import android.opengl.GLES20;
import android.opengl.GLUtils;
import android.util.Log;

public class QmCameraBaseGLFilter {
	private static final String TAG = QmCameraBaseGLFilter.class
			.getSimpleName();
	private static final boolean DEBUG = true;

	private int mProgram = 0;
	private int mShaderFragment = 0;
	private final HashMap<String, Integer> mShaderHandleMap = new HashMap<String, Integer>();
	private int mShaderVertex = 0;

	public void deleteProgram() {
		GLES20.glDeleteShader(mShaderFragment);
		GLES20.glDeleteShader(mShaderVertex);
		GLES20.glDeleteProgram(mProgram);
		mProgram = mShaderVertex = mShaderFragment = 0;
	}

	public int getHandle(String name) {
		if (mShaderHandleMap.containsKey(name)) {
			return mShaderHandleMap.get(name);
		}
		int handle = GLES20.glGetAttribLocation(mProgram, name);
		if (handle == -1) {
			handle = GLES20.glGetUniformLocation(mProgram, name);
		}
		if (handle == -1) {
			LOGD("GLslShader: Could not get attrib location for " + name);
		} else {
			mShaderHandleMap.put(name, handle);
		}
		return handle;
	}

	public int[] getHandles(String... names) {
		int[] res = new int[names.length];
		for (int i = 0; i < names.length; ++i) {
			res[i] = getHandle(names[i]);
		}
		return res;
	}

	private int loadShader(int shaderType, String source) {
		int shader = GLES20.glCreateShader(shaderType);
		if (shader != 0) {
			GLES20.glShaderSource(shader, source);
			GLES20.glCompileShader(shader);
			int[] compiled = new int[1];
			GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, compiled, 0);
			if (compiled[0] == 0) {
				String error = GLES20.glGetShaderInfoLog(shader);
				GLES20.glDeleteShader(shader);
				LOGD("gl compile program error: " + error);
			}
		}
		return shader;
	}

	public void setProgram(String vertexSource, String fragmentSource) {
		mShaderVertex = loadShader(GLES20.GL_VERTEX_SHADER, vertexSource);
		mShaderFragment = loadShader(GLES20.GL_FRAGMENT_SHADER, fragmentSource);
		int program = GLES20.glCreateProgram();
		if (program != 0) {
			GLES20.glAttachShader(program, mShaderVertex);
			GLES20.glAttachShader(program, mShaderFragment);
			GLES20.glLinkProgram(program);
			int[] linkStatus = new int[1];
			GLES20.glGetProgramiv(program, GLES20.GL_LINK_STATUS, linkStatus, 0);
			if (linkStatus[0] != GLES20.GL_TRUE) {
				LOGD("gl link program error: "
						+ GLES20.glGetProgramInfoLog(program));
				deleteProgram();
			}
		}
		mProgram = program;
		mShaderHandleMap.clear();
	}

	public void useProgram() {
		GLES20.glUseProgram(mProgram);
	}

	protected int loadTexture(final Bitmap img, final int usedTexId,
			final boolean recycle) {
		int textures[] = new int[1];
		if (usedTexId == -1) {
			GLES20.glGenTextures(1, textures, 0);
			GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textures[0]);
			GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D,
					GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
			GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D,
					GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
			GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D,
					GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
			GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D,
					GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);
			GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, img, 0);
		} else {
			GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, usedTexId);
			GLUtils.texSubImage2D(GLES20.GL_TEXTURE_2D, 0, 0, 0, img);
			textures[0] = usedTexId;
		}
		if (recycle) {
			img.recycle();
		}
		return textures[0];
	}

	private void LOGD(String message) {
		if (DEBUG) {
			Log.i(TAG, message);
		}
	}

}
