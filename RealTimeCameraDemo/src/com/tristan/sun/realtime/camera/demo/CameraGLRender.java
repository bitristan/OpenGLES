package com.tristan.sun.realtime.camera.demo;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.hardware.Camera;
import android.hardware.Camera.PreviewCallback;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;
import android.util.Log;

public class CameraGLRender implements Renderer, PreviewCallback {

	static final float TEXTURE_NO_ROTATION[] = { 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f, };
	static final float TEXTURE_ROTATED_90[] = { 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f, };
	static final float TEXTURE_ROTATED_180[] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, };
	static final float TEXTURE_ROTATED_270[] = { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 1.0f, 1.0f, };

	float textureCoordinate[] = TEXTURE_NO_ROTATION;

	float vertices[] = new float[] { -1.0f, 1.0f, 0.0f, -1.0f, -1.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f };

	FloatBuffer mVerticesBuffer;
	FloatBuffer mTextureCoordinateBuffer;

	private int mPositionHandler;
	private int mTextureCoordinateHandler;
	private int mGLUniformTexture;
	private int mGLUniformUvTexture;

	private int mProgramHandler;

	private int mTextureId;
	private int mUvTextureId;

	Context mContext;
	private int mWidth;
	private int mHeight;

	byte[] y;
	byte[] uv;
	
	GLSurfaceView glSurfaceView;
	
	boolean hasPreview = false;

	public CameraGLRender(Context context, GLSurfaceView view) {
		mContext = context;
		glSurfaceView = view;

		mVerticesBuffer = ByteBuffer.allocateDirect(vertices.length * 4)
				.order(ByteOrder.nativeOrder()).asFloatBuffer();
		mVerticesBuffer.put(vertices).position(0);

		mTextureCoordinateBuffer = ByteBuffer
				.allocateDirect(textureCoordinate.length * 4)
				.order(ByteOrder.nativeOrder()).asFloatBuffer();
		mTextureCoordinateBuffer.put(textureCoordinate).position(0);
	}

	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		System.out.println("onSurfaceCreated");
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
		GLES20.glTexImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_LUMINANCE,
				mWidth, mHeight, 0, GLES20.GL_LUMINANCE,
				GLES20.GL_UNSIGNED_BYTE, ByteBuffer.wrap(y));

		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textures[1]);
		GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D,
				GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
		GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D,
				GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
		GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S,
				GLES20.GL_CLAMP_TO_EDGE);
		GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T,
				GLES20.GL_CLAMP_TO_EDGE);
		GLES20.glTexImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_LUMINANCE_ALPHA,
				mWidth / 2, mHeight / 2, 0, GLES20.GL_LUMINANCE_ALPHA,
				GLES20.GL_UNSIGNED_BYTE, ByteBuffer.wrap(uv));

		mTextureId = textures[0];
		mUvTextureId = textures[1];

		int vertexShaderHandle = GLES20.glCreateShader(GLES20.GL_VERTEX_SHADER);
		if (vertexShaderHandle != 0) {
			GLES20.glShaderSource(vertexShaderHandle,
					loadStringFromRaw(mContext, R.raw.filter_vs));
			GLES20.glCompileShader(vertexShaderHandle);
			final int[] compileStatus = new int[1];
			GLES20.glGetShaderiv(vertexShaderHandle, GLES20.GL_COMPILE_STATUS,
					compileStatus, 0);
			if (compileStatus[0] == 0) {
				GLES20.glDeleteShader(vertexShaderHandle);
				Log.d("OpenGLES",
						"Compilation\n"
								+ GLES20.glGetShaderInfoLog(vertexShaderHandle));
				vertexShaderHandle = 0;
			}
		}

		if (vertexShaderHandle == 0) {
			throw new RuntimeException("Error creating vertex shader.");
		}

		int fragmentShaderHandle = GLES20
				.glCreateShader(GLES20.GL_FRAGMENT_SHADER);
		if (fragmentShaderHandle != 0) {
			GLES20.glShaderSource(fragmentShaderHandle,
					loadStringFromRaw(mContext, R.raw.filter_fs));
			GLES20.glCompileShader(fragmentShaderHandle);
			final int[] compileStatus = new int[1];
			GLES20.glGetShaderiv(fragmentShaderHandle,
					GLES20.GL_COMPILE_STATUS, compileStatus, 0);

			if (compileStatus[0] == 0) {
				GLES20.glDeleteShader(fragmentShaderHandle);
				Log.d("OpenGLES",
						"Compilation\n"
								+ GLES20.glGetShaderInfoLog(fragmentShaderHandle));
				fragmentShaderHandle = 0;
			}
		}

		if (fragmentShaderHandle == 0) {
			throw new RuntimeException("Error creating fragment shader.");
		}

		int programHandle = GLES20.glCreateProgram();
		if (programHandle != 0) {
			GLES20.glAttachShader(programHandle, vertexShaderHandle);
			GLES20.glAttachShader(programHandle, fragmentShaderHandle);
			GLES20.glBindAttribLocation(programHandle, 0, "position");
			GLES20.glBindAttribLocation(programHandle, 1,
					"inputTextureCoordinate");
			GLES20.glLinkProgram(programHandle);
			final int[] linkStatus = new int[1];
			GLES20.glGetProgramiv(programHandle, GLES20.GL_LINK_STATUS,
					linkStatus, 0);
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
		mGLUniformUvTexture = GLES20.glGetUniformLocation(programHandle,
				"uvTexture");

		GLES20.glUseProgram(programHandle);
		mProgramHandler = programHandle;
	}

	public void onDrawFrame(GL10 gl) {
		System.out.println("onDrawFrame width = " + mWidth + "; height = " + mHeight);
		
		if (!hasPreview) {
			return;
		}
		
		GLES20.glUseProgram(mProgramHandler);
		GLES20.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
		GLES20.glVertexAttribPointer(mPositionHandler, 3, GLES20.GL_FLOAT,
				false, 0, mVerticesBuffer);
		GLES20.glEnableVertexAttribArray(mPositionHandler);
		GLES20.glVertexAttribPointer(mTextureCoordinateHandler, 2,
				GLES20.GL_FLOAT, false, 0, mTextureCoordinateBuffer);
		GLES20.glEnableVertexAttribArray(mTextureCoordinateHandler);

		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mTextureId);
		GLES20.glTexSubImage2D(GLES20.GL_TEXTURE_2D, 0, 0, 0, mWidth, mHeight,
				GLES20.GL_LUMINANCE, GLES20.GL_UNSIGNED_BYTE,
				ByteBuffer.wrap(y));

		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mUvTextureId);
		GLES20.glTexSubImage2D(GLES20.GL_TEXTURE_2D, 0, 0, 0, mWidth / 2,
				mHeight / 2, GLES20.GL_LUMINANCE_ALPHA,
				GLES20.GL_UNSIGNED_BYTE, ByteBuffer.wrap(uv));

		GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mTextureId);
		GLES20.glUniform1i(mGLUniformTexture, 0);

		GLES20.glActiveTexture(GLES20.GL_TEXTURE1);
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mUvTextureId);
		GLES20.glUniform1i(mGLUniformUvTexture, 1);

		GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 4);
		GLES20.glDisableVertexAttribArray(mPositionHandler);
		GLES20.glDisableVertexAttribArray(mTextureCoordinateHandler);
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0);
	}

	public void onSurfaceChanged(GL10 gl, int width, int height) {
		System.out.println("onSurfaceChanged");
		GLES20.glViewport(0, 0, width, height);
	}

	public String loadStringFromRaw(Context context, int rawId) {
		StringBuilder sb = new StringBuilder();
		String l;
		try {
			InputStream is = context.getResources().openRawResource(rawId);
			BufferedReader br = new BufferedReader(new InputStreamReader(is));
			while ((l = br.readLine()) != null) {
				sb.append(l);
			}
			br.close();
		} catch (IOException e) {
			e.printStackTrace();
		}

		return sb.toString();
	}

	public void setTextureSize(int width, int height) {
		mWidth = width;
		mHeight = height;

		y = new byte[width * height];
		uv = new byte[width * height / 2];
	}

	@Override
	public void onPreviewFrame(byte[] data, Camera camera) {
		System.out.println("onPreviewFrame");
		
		hasPreview = true;

		int length = mWidth * mHeight;
		System.arraycopy(data, 0, y, 0, length);
		System.arraycopy(data, length, uv, 0, length / 2);
		
		glSurfaceView.requestRender();
	}

}
