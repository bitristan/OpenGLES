package com.robert.surfacetexture.demo;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.SurfaceTexture;
import android.opengl.GLES11Ext;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.Matrix;
import android.util.AttributeSet;

import com.renren.qm.camera.R;

public class QmCameraRender extends GLSurfaceView implements
		GLSurfaceView.Renderer, SurfaceTexture.OnFrameAvailableListener {
	private Context mContext;

	private final float mAspectRatio[] = new float[2];
	private FloatBuffer mFullQuadVertices;
	private Observer mObserver;
	private QmCameraBaseGLFilter mShaderCopyOes = new QmCameraBaseGLFilter();;

	private SurfaceTexture mSurfaceTexture;
	private boolean mSurfaceTextureUpdate;
	private final float[] mTransformM = new float[16];
	private int mWidth, mHeight;

	public final float mOrientationM[] = new float[16];

	private int mExternalOESTextureId;

	public QmCameraRender(Context context) {
		super(context);
		init(context);
	}

	public QmCameraRender(Context context, AttributeSet attrs) {
		super(context, attrs);
		init(context);
	}

	private void init(Context context) {
		mContext = context;

		final float FULL_QUAD_COORDS[] = new float[] { -1.0f, -1.0f, 1.0f,
				-1.0f, -1.0f, 1.0f, 1.0f, 1.0f };
		mFullQuadVertices = ByteBuffer
				.allocateDirect(FULL_QUAD_COORDS.length * 4)
				.order(ByteOrder.nativeOrder()).asFloatBuffer();
		mFullQuadVertices.put(FULL_QUAD_COORDS).position(0);

		setPreserveEGLContextOnPause(true);
		setEGLContextClientVersion(2);
		setRenderer(this);
		setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
	}

	@SuppressLint("InlinedApi")
	@Override
	public void onDrawFrame(GL10 unused) {
		GLES20.glClearColor(.5f, .5f, .5f, 1f);
		GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);

		if (mSurfaceTextureUpdate) {
			mSurfaceTextureUpdate = false;
			mSurfaceTexture.updateTexImage();
			mSurfaceTexture.getTransformMatrix(mTransformM);

			GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, 0);
			GLES20.glViewport(0, 0, mWidth, mHeight);

			mShaderCopyOes.useProgram();

			int uOrientationM = mShaderCopyOes.getHandle("uOrientationM");
			int uTransformM = mShaderCopyOes.getHandle("uTransformM");

			Matrix.setRotateM(mOrientationM, 0, 90, 0, 0, 1);
			GLES20.glUniformMatrix4fv(uOrientationM, 1, false, mOrientationM, 0);
			GLES20.glUniformMatrix4fv(uTransformM, 1, false, mTransformM, 0);

			GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
			GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES,
					mExternalOESTextureId);
			GLES20.glUniform1i(mShaderCopyOes.getHandle("sTexture"), 0);

			renderQuad(mShaderCopyOes.getHandle("position"));
		}
	}

	@Override
	public void onFrameAvailable(SurfaceTexture surfaceTexture) {
		mSurfaceTextureUpdate = true;
		requestRender();
	}

	@Override
	public void onSurfaceChanged(GL10 unused, int width, int height) {
		mWidth = width;
		mHeight = height;

		mAspectRatio[0] = (float) Math.min(mWidth, mHeight) / mWidth;
		mAspectRatio[1] = (float) Math.min(mWidth, mHeight) / mHeight;

		SurfaceTexture oldSurfaceTexture = mSurfaceTexture;
		mSurfaceTexture = new SurfaceTexture(mExternalOESTextureId);
		mSurfaceTexture.setOnFrameAvailableListener(this);
		if (mObserver != null) {
			mObserver.onSurfaceTextureCreated(mSurfaceTexture);
		}
		if (oldSurfaceTexture != null) {
			oldSurfaceTexture.release();
		}
	}

	@SuppressLint("InlinedApi")
	@Override
	public synchronized void onSurfaceCreated(GL10 unused, EGLConfig config) {
		mShaderCopyOes.setProgram(
				loadRawString(mContext, R.raw.shader_copy_oes_vertex),
				loadRawString(mContext, R.raw.shader_copy_oes_fragment));

		int textures[] = new int[1];
		GLES20.glGenTextures(1, textures, 0);
		GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, textures[0]);
		GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES,
				GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
		GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES,
				GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);
		GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES,
				GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_NEAREST);
		GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES,
				GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
		mExternalOESTextureId = textures[0];
	}

	private void renderQuad(int aPosition) {
		GLES20.glVertexAttribPointer(aPosition, 2, GLES20.GL_FLOAT, false, 0,
				mFullQuadVertices);
		GLES20.glEnableVertexAttribArray(aPosition);
		GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 4);
		GLES20.glDisableVertexAttribArray(aPosition);
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0);
	}

	public String loadRawString(Context context, int rawId) {
		String result = null;
		InputStream is = context.getResources().openRawResource(rawId);
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		byte[] buf = new byte[1024];
		int len;
		try {
			while ((len = is.read(buf)) != -1) {
				baos.write(buf, 0, len);
			}
			result = baos.toString();

			is.close();
			baos.close();
		} catch (IOException e) {
		}

		return result;
	}

	public void setObserver(Observer observer) {
		mObserver = observer;
	}

	public interface Observer {
		public void onSurfaceTextureCreated(SurfaceTexture surfaceTexture);

		public void onPhotoTextureCreated();
	}
}
