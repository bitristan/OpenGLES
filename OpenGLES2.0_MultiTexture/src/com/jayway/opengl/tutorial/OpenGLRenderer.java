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

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
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

	float textureCoordinate[] = TEXTURE_ROTATED_270;

	float vertices[] = new float[] { -1.0f, 1.0f, 0.0f, -1.0f, -1.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f };

	FloatBuffer mVerticesBuffer;
	FloatBuffer mTextureCoordinateBuffer;

	private int mPositionHandler;
	private int mTextureCoordinateHandler;
	private int mGLUniformTexture;
	private int mGLUniformTexture2;

	private int mProgramHandler;

	private int mTextureId;
	private int mTextureId2;

	Context mContext;

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
		GLES20.glClearColor(0, 0, 0, 1);

		int textures[] = new int[2];
		GLES20.glGenTextures(2, textures, 0);
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
		GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, BitmapFactory
				.decodeResource(mContext.getResources(), R.drawable.test,
						options), 0);

		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textures[1]);
		GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D,
				GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
		GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D,
				GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
		GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S,
				GLES20.GL_CLAMP_TO_EDGE);
		GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T,
				GLES20.GL_CLAMP_TO_EDGE);
		GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, BitmapFactory
				.decodeResource(mContext.getResources(), R.drawable.jay,
						options), 0);
		mTextureId = textures[0];
		mTextureId2 = textures[1];

		int vertexShaderHandle = GLES20.glCreateShader(GLES20.GL_VERTEX_SHADER);
		if (vertexShaderHandle != 0) {
			// Load the shader
			GLES20.glShaderSource(vertexShaderHandle, mContext.getResources()
					.getString(R.string.filter_vs));
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

		System.out.println("success compile vertex shader");

		// Load in the fragment shader shader.
		int fragmentShaderHandle = GLES20
				.glCreateShader(GLES20.GL_FRAGMENT_SHADER);
		if (fragmentShaderHandle != 0) {
			// Pass in the shader source.
			GLES20.glShaderSource(fragmentShaderHandle, mContext.getResources()
					.getString(R.string.filter_fs));
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

		System.out.println("success compile fragment shader");

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

		mPositionHandler = GLES20
				.glGetAttribLocation(programHandle, "position");
		mTextureCoordinateHandler = GLES20.glGetAttribLocation(programHandle,
				"inputTextureCoordinate");
		mGLUniformTexture = GLES20.glGetUniformLocation(programHandle,
				"inputImageTexture");
		mGLUniformTexture2 = GLES20.glGetUniformLocation(programHandle,
				"inputImageTexture2");

		// Tell OpenGL to use this program when rendering.
		GLES20.glUseProgram(programHandle);
		mProgramHandler = programHandle;

		Log.d("OpenGLES", "Init success.");
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * android.opengl.GLSurfaceView.Renderer#onDrawFrame(javax.microedition.
	 * khronos.opengles.GL10)
	 */
	public void onDrawFrame(GL10 gl) {
		GLES20.glUseProgram(mProgramHandler);
		// Clears the screen and depth buffer.
		GLES20.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
		// Draw our scene.
		GLES20.glVertexAttribPointer(mPositionHandler, 3, GLES20.GL_FLOAT,
				false, 0, mVerticesBuffer);
		GLES20.glEnableVertexAttribArray(mPositionHandler);
		GLES20.glVertexAttribPointer(mTextureCoordinateHandler, 2,
				GLES20.GL_FLOAT, false, 0, mTextureCoordinateBuffer);
		GLES20.glEnableVertexAttribArray(mTextureCoordinateHandler);

		GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mTextureId);
		GLES20.glUniform1i(mGLUniformTexture, 0);
		
		GLES20.glActiveTexture(GLES20.GL_TEXTURE1);
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mTextureId2);
		GLES20.glUniform1i(mGLUniformTexture2, 1);

		GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 4);
		GLES20.glDisableVertexAttribArray(mPositionHandler);
		GLES20.glDisableVertexAttribArray(mTextureCoordinateHandler);
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0);
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
	}

}
