/**
 * Copyright 2010 Per-Erik Bergman (per-erik.bergman@jayway.com)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.jayway.opengl.tutorial;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.BitmapFactory;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView.Renderer;
import android.opengl.GLUtils;
import android.util.Log;

public class OpenGLRenderer implements Renderer {
	static final float TEXTURE_NO_ROTATION[] = { 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f, };
	static final float TEXTURE_ROTATED_90[] = { 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f, };
	static final float TEXTURE_ROTATED_180[] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, };
	static final float TEXTURE_ROTATED_270[] = { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 1.0f, 1.0f, };

	float textureCoordinate[] = TEXTURE_NO_ROTATION;

	float vertices[] = new float[] { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f };

	FloatBuffer mVerticesBuffer;
	FloatBuffer mTextureCoordinateBuffer;

	private final static int FILTER_COUNT = 2;

	private int mPositionHandler[] = new int[FILTER_COUNT];
	private int mTextureCoordinateHandler[] = new int[FILTER_COUNT];
	private int mGLUniformTexture[] = new int[FILTER_COUNT];

	private int mProgramHandler[] = new int[FILTER_COUNT];

	private int mTextureId;

	private int[] mFrameBuffer = new int[FILTER_COUNT - 1];
	private int[] mFrameBufferTextures = new int[FILTER_COUNT - 1];

	Context mContext;

	private long mStartTime;

	public OpenGLRenderer(Context context) {
		mContext = context;

		mVerticesBuffer = ByteBuffer.allocateDirect(vertices.length * 4)
				.order(ByteOrder.nativeOrder()).asFloatBuffer();
		mVerticesBuffer.put(vertices).position(0);

		mTextureCoordinateBuffer = ByteBuffer
				.allocateDirect(textureCoordinate.length * 4)
				.order(ByteOrder.nativeOrder()).asFloatBuffer();
		mTextureCoordinateBuffer.put(textureCoordinate).position(0);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * android.opengl.GLSurfaceView.Renderer#onSurfaceCreated(javax.microedition
	 * .khronos.opengles.GL10, javax.microedition.khronos.egl.EGLConfig)
	 */
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		mStartTime = System.nanoTime();
		GLES20.glClearColor(0, 0, 0, 1);

		int textures[] = new int[1];
		GLES20.glGenTextures(1, textures, 0);
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textures[0]);
		GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D,
				GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
		GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D,
				GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
		GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S,
				GLES20.GL_CLAMP_TO_EDGE);
		GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T,
				GLES20.GL_CLAMP_TO_EDGE);
		BitmapFactory.Options options = new BitmapFactory.Options();
		options.inPreferredConfig = Config.ARGB_8888;
		Bitmap bitmap = BitmapFactory.decodeResource(mContext.getResources(),
				R.drawable.path, options);
		GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, bitmap, 0);
		mTextureId = textures[0];
		bitmap.recycle();

		int programHandle = linkProgram(
				loadRawString(mContext, R.raw.filter_vs),
				loadRawString(mContext, R.raw.filter_fs));
		mPositionHandler[0] = GLES20.glGetAttribLocation(programHandle,
				"position");
		mTextureCoordinateHandler[0] = GLES20.glGetAttribLocation(
				programHandle, "inputTextureCoordinate");
		mGLUniformTexture[0] = GLES20.glGetUniformLocation(programHandle,
				"inputImageTexture");
		mProgramHandler[0] = programHandle;

		programHandle = linkProgram(loadRawString(mContext, R.raw.filter_vs),
				loadRawString(mContext, R.raw.filter_pro_fs));
		mPositionHandler[1] = GLES20.glGetAttribLocation(programHandle,
				"position");
		mTextureCoordinateHandler[1] = GLES20.glGetAttribLocation(
				programHandle, "inputTextureCoordinate");
		mGLUniformTexture[1] = GLES20.glGetUniformLocation(programHandle,
				"inputImageTexture");
		mProgramHandler[1] = programHandle;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * android.opengl.GLSurfaceView.Renderer#onDrawFrame(javax.microedition.
	 * khronos.opengles.GL10)
	 */
	public void onDrawFrame(GL10 gl) {
		GLES20.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
		
		GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, mFrameBuffer[0]);
		GLES20.glUseProgram(mProgramHandler[0]);
		GLES20.glVertexAttribPointer(mPositionHandler[0], 3, GLES20.GL_FLOAT,
				false, 0, mVerticesBuffer);
		GLES20.glEnableVertexAttribArray(mPositionHandler[0]);
		GLES20.glVertexAttribPointer(mTextureCoordinateHandler[0], 2,
				GLES20.GL_FLOAT, false, 0, mTextureCoordinateBuffer);
		GLES20.glEnableVertexAttribArray(mTextureCoordinateHandler[0]);

		GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mTextureId);
		GLES20.glUniform1i(mGLUniformTexture[0], 0);

		GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 4);
		GLES20.glDisableVertexAttribArray(mPositionHandler[0]);
		GLES20.glDisableVertexAttribArray(mTextureCoordinateHandler[0]);
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0);
		GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, 0);
		
		mTextureId = mFrameBufferTextures[0];
		
		GLES20.glUseProgram(mProgramHandler[1]);
		GLES20.glVertexAttribPointer(mPositionHandler[1], 3, GLES20.GL_FLOAT,
				false, 0, mVerticesBuffer);
		GLES20.glEnableVertexAttribArray(mPositionHandler[1]);
		GLES20.glVertexAttribPointer(mTextureCoordinateHandler[1], 2,
				GLES20.GL_FLOAT, false, 0, mTextureCoordinateBuffer);
		GLES20.glEnableVertexAttribArray(mTextureCoordinateHandler[1]);

		GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mTextureId);
		GLES20.glUniform1i(mGLUniformTexture[1], 0);

		GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 4);
		GLES20.glDisableVertexAttribArray(mPositionHandler[1]);
		GLES20.glDisableVertexAttribArray(mTextureCoordinateHandler[1]);
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0);
		
		long end = System.nanoTime();
		System.out.println("time is: " + (end - mStartTime));
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * android.opengl.GLSurfaceView.Renderer#onSurfaceChanged(javax.microedition
	 * .khronos.opengles.GL10, int, int)
	 */
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		GLES20.glViewport(0, 0, width, height);

		for (int i = 0; i < FILTER_COUNT - 1; i++) {
			GLES20.glGenFramebuffers(1, mFrameBuffer, i);
			GLES20.glGenTextures(1, mFrameBufferTextures, i);
			GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mFrameBufferTextures[i]);
			GLES20.glTexImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_RGBA, width,
					height, 0, GLES20.GL_RGBA, GLES20.GL_UNSIGNED_BYTE, null);
			GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D,
					GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
			GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D,
					GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
			GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D,
					GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
			GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D,
					GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);

			GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, mFrameBuffer[i]);
			GLES20.glFramebufferTexture2D(GLES20.GL_FRAMEBUFFER,
					GLES20.GL_COLOR_ATTACHMENT0, GLES20.GL_TEXTURE_2D,
					mFrameBufferTextures[i], 0);

			GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0);
			GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, 0);
		}
	}

	public static String loadRawString(Context context, int rawId) {
		InputStream is = context.getResources().openRawResource(rawId);
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		try {
			byte[] buf = new byte[1024];
			int len = -1;
			while ((len = is.read(buf)) != -1) {
				baos.write(buf, 0, len);
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
		return baos.toString();
	}

	public int linkProgram(String vetexShader, String fragmentShader) {
		int vertexShaderHandle = GLES20.glCreateShader(GLES20.GL_VERTEX_SHADER);
		if (vertexShaderHandle != 0) {
			// Load the shader
			GLES20.glShaderSource(vertexShaderHandle, vetexShader);
			// Compile the shader.
			GLES20.glCompileShader(vertexShaderHandle);
			// Get the compilation status.
			final int[] compileStatus = new int[1];
			GLES20.glGetShaderiv(vertexShaderHandle, GLES20.GL_COMPILE_STATUS,
					compileStatus, 0);
			// If the compilation failed, delete the shader.
			if (compileStatus[0] == 0) {
				Log.d("OpenGLES",
						"Compilation:"
								+ GLES20.glGetShaderInfoLog(vertexShaderHandle));
				GLES20.glDeleteShader(vertexShaderHandle);
				vertexShaderHandle = 0;
			}
		}

		if (vertexShaderHandle == 0) {
			throw new RuntimeException("Error creating vertex shader.");
		}

		// Load in the fragment shader shader.
		int fragmentShaderHandle = GLES20
				.glCreateShader(GLES20.GL_FRAGMENT_SHADER);
		if (fragmentShaderHandle != 0) {
			// Pass in the shader source.
			GLES20.glShaderSource(fragmentShaderHandle, fragmentShader);
			// Compile the shader.
			GLES20.glCompileShader(fragmentShaderHandle);
			// Get the compilation status.
			final int[] compileStatus = new int[1];
			GLES20.glGetShaderiv(fragmentShaderHandle,
					GLES20.GL_COMPILE_STATUS, compileStatus, 0);

			// If the compilation failed, delete the shader.
			if (compileStatus[0] == 0) {
				Log.d("OpenGLES",
						"Compilation:"
								+ GLES20.glGetShaderInfoLog(fragmentShaderHandle));
				GLES20.glDeleteShader(fragmentShaderHandle);
				fragmentShaderHandle = 0;
			}
		}

		if (fragmentShaderHandle == 0) {
			throw new RuntimeException("Error creating fragment shader.");
		}

		// Create a program object and store the handle to it.
		int programHandle = GLES20.glCreateProgram();
		if (programHandle != 0) {
			// Bind the vertex shader to the program.
			GLES20.glAttachShader(programHandle, vertexShaderHandle);
			// Bind the fragment shader to the program.
			GLES20.glAttachShader(programHandle, fragmentShaderHandle);
			// Bind attributes
			GLES20.glBindAttribLocation(programHandle, 0, "position");
			// GLES20.glBindAttribLocation(programHandle, 1, "a_Color");
			GLES20.glBindAttribLocation(programHandle, 1,
					"inputTextureCoordinate");
			// Link the two shaders together into a program.
			GLES20.glLinkProgram(programHandle);
			// Get the link status.
			final int[] linkStatus = new int[1];
			GLES20.glGetProgramiv(programHandle, GLES20.GL_LINK_STATUS,
					linkStatus, 0);
			// If the link failed, delete the program.
			if (linkStatus[0] == 0) {
				GLES20.glDeleteProgram(programHandle);
				programHandle = 0;
			}
		}

		if (programHandle == 0) {
			throw new RuntimeException("Error creating program.");
		}

		return programHandle;
	}
}
