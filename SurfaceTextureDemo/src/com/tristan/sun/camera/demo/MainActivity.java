package com.tristan.sun.camera.demo;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.content.Context;
import android.graphics.SurfaceTexture;
import android.graphics.SurfaceTexture.OnFrameAvailableListener;
import android.hardware.Camera;
import android.opengl.GLES11Ext;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;
import android.opengl.Matrix;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;

public class MainActivity extends Activity {

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);
		setContentView(new MyView(this));
	}

	private class MyView extends GLSurfaceView implements Renderer,
			OnFrameAvailableListener {
		private static final int COORDS_PER_VERTEX = 2;
		private float squareVertices[] = { -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,
				-1.0f, 1.0f, 1.0f };

		private short drawOrder[] = { 0, 1, 2, 0, 2, 3 };

		private float textureVertices[] = { 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				0.0f, 0.0f };

		private final int vertexStride = COORDS_PER_VERTEX * 4;

		private SurfaceTexture mSurfaceTexture;
		private Camera mCamera;

		private FloatBuffer vertexBuffer, textureVerticesBuffer;
		private ShortBuffer drawListBuffer;
		private int mProgram;
		private int mPositionHandle;
		private int mGLUniformTexture;
		private int mTextureCoordHandle;
		private int mGLUniformTransformMHandler;
		private int mGLUniformOrientationHandler;

		private float[] mOrientionM = new float[16];

		private int mTextureId;

		public MyView(Context context) {
			super(context);

			setEGLContextClientVersion(2);
			setRenderer(this);
			setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
		}

		@Override
		public void onSurfaceCreated(GL10 gl, EGLConfig config) {
			ByteBuffer bb = ByteBuffer
					.allocateDirect(squareVertices.length * 4);
			bb.order(ByteOrder.nativeOrder());
			vertexBuffer = bb.asFloatBuffer();
			vertexBuffer.put(squareVertices);
			vertexBuffer.position(0);

			ByteBuffer dlb = ByteBuffer.allocateDirect(drawOrder.length * 2);
			dlb.order(ByteOrder.nativeOrder());
			drawListBuffer = dlb.asShortBuffer();
			drawListBuffer.put(drawOrder);
			drawListBuffer.position(0);

			ByteBuffer bb2 = ByteBuffer
					.allocateDirect(textureVertices.length * 4);
			bb2.order(ByteOrder.nativeOrder());
			textureVerticesBuffer = bb2.asFloatBuffer();
			textureVerticesBuffer.put(textureVertices);
			textureVerticesBuffer.position(0);

			int vertexShader = loadShader(GLES20.GL_VERTEX_SHADER,
					loadRawString(R.raw.copy_oes_vs));
			int fragmentShader = loadShader(GLES20.GL_FRAGMENT_SHADER,
					loadRawString(R.raw.copy_oes_fs));

			mProgram = GLES20.glCreateProgram();
			GLES20.glAttachShader(mProgram, vertexShader);
			GLES20.glAttachShader(mProgram, fragmentShader);
			GLES20.glLinkProgram(mProgram);

			mTextureId = createTexture();
			GLES20.glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
			startCamera(mTextureId);
		}

		@Override
		public void onSurfaceChanged(GL10 gl, int width, int height) {
			GLES20.glViewport(0, 0, width, height);
		}

		@Override
		public void onDrawFrame(GL10 gl) {
			float[] mtx = new float[16];
			GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT
					| GLES20.GL_DEPTH_BUFFER_BIT);
			mSurfaceTexture.updateTexImage();
			mSurfaceTexture.getTransformMatrix(mtx);

			GLES20.glUseProgram(mProgram);

			GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
			GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, mTextureId);
			GLES20.glUniform1i(mGLUniformTexture, 0);

			mPositionHandle = GLES20.glGetAttribLocation(mProgram, "position");
			GLES20.glEnableVertexAttribArray(mPositionHandle);
			GLES20.glVertexAttribPointer(mPositionHandle, COORDS_PER_VERTEX,
					GLES20.GL_FLOAT, false, vertexStride, vertexBuffer);

			mTextureCoordHandle = GLES20.glGetAttribLocation(mProgram,
					"inputTextureCoordinate");
			GLES20.glEnableVertexAttribArray(mTextureCoordHandle);
			GLES20.glVertexAttribPointer(mTextureCoordHandle,
					COORDS_PER_VERTEX, GLES20.GL_FLOAT, false, vertexStride,
					textureVerticesBuffer);

			mGLUniformTexture = GLES20.glGetUniformLocation(mProgram,
					"s_texture");

			mGLUniformTransformMHandler = GLES20.glGetUniformLocation(mProgram,
					"uTransformM");
			GLES20.glUniformMatrix4fv(mGLUniformTransformMHandler, 1, false,
					mtx, 0);

			mGLUniformOrientationHandler = GLES20.glGetUniformLocation(
					mProgram, "uOrientationM");
			Matrix.setRotateM(mOrientionM, 0, 270, 0, 0, 1);
			GLES20.glUniformMatrix4fv(mGLUniformOrientationHandler, 1, false,
					mOrientionM, 0);

			GLES20.glDrawElements(GLES20.GL_TRIANGLES, drawOrder.length,
					GLES20.GL_UNSIGNED_SHORT, drawListBuffer);

			GLES20.glDisableVertexAttribArray(mPositionHandle);
			GLES20.glDisableVertexAttribArray(mTextureCoordHandle);
		}

		@Override
		public void onFrameAvailable(SurfaceTexture surfaceTexture) {
			requestRender();
		}

		public int createTexture() {
			int[] texture = new int[1];

			GLES20.glGenTextures(1, texture, 0);
			GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, texture[0]);
			GLES20.glTexParameterf(GLES11Ext.GL_TEXTURE_EXTERNAL_OES,
					GL10.GL_TEXTURE_MIN_FILTER, GL10.GL_LINEAR);
			GLES20.glTexParameterf(GLES11Ext.GL_TEXTURE_EXTERNAL_OES,
					GL10.GL_TEXTURE_MAG_FILTER, GL10.GL_LINEAR);
			GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES,
					GL10.GL_TEXTURE_WRAP_S, GL10.GL_CLAMP_TO_EDGE);
			GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES,
					GL10.GL_TEXTURE_WRAP_T, GL10.GL_CLAMP_TO_EDGE);

			return texture[0];
		}

		public int loadShader(int type, String shaderCode) {
			int shader = GLES20.glCreateShader(type);

			GLES20.glShaderSource(shader, shaderCode);
			GLES20.glCompileShader(shader);

			return shader;
		}

		public void startCamera(int texture) {
			mSurfaceTexture = new SurfaceTexture(texture);
			mSurfaceTexture.setOnFrameAvailableListener(this);

			mCamera = Camera.open();
			try {
				mCamera.setPreviewTexture(mSurfaceTexture);
				mCamera.startPreview();
			} catch (IOException ioe) {
				ioe.printStackTrace();
			}
		}

		public String loadRawString(int rawId) {
			InputStream is = getContext().getResources().openRawResource(rawId);
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
	}
}
