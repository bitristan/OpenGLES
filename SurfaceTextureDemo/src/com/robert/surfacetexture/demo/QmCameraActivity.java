package com.robert.surfacetexture.demo;

import android.app.Activity;
import android.content.Context;
import android.graphics.SurfaceTexture;
import android.os.Bundle;
import android.util.Log;
import android.view.OrientationEventListener;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;

import com.renren.qm.camera.R;

public class QmCameraActivity extends Activity {
	private static final String TAG = QmCameraActivity.class.getSimpleName();
	private static final boolean DEBUG = true;

	QmCamera mQmCamera;
	OrientationObserver mOrientationObserver;
	RendererObserver mRenderObserver = new RendererObserver();
	QmCameraRender mQmCameraRender;

	Button mCameraNextBT;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
		getWindow().clearFlags(
				WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN);

		setContentView(R.layout.camera_main);

		mQmCameraRender = (QmCameraRender) findViewById(R.id.camera_surface);

		int width = getResources().getDisplayMetrics().widthPixels;
		mQmCameraRender.getLayoutParams().width = width;
		mQmCameraRender.getLayoutParams().height = width;

		mOrientationObserver = new OrientationObserver(this);

		mQmCameraRender.setObserver(mRenderObserver);

		mQmCamera = new QmCamera(this.getApplicationContext());
		mQmCamera.setCameraFront(false);

		mCameraNextBT = (Button) findViewById(R.id.ib_camera_next);
		mCameraNextBT.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				mQmCamera.setCameraFront(!mQmCamera.isCameraFront());
			}
		});
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
	}

	@Override
	protected void onPause() {
		super.onPause();

		mQmCamera.onPause();
		mQmCameraRender.onPause();
		mOrientationObserver.disable();
	}

	@Override
	protected void onResume() {
		super.onResume();

		mQmCamera.onResume();
		mQmCameraRender.onResume();
		if (mOrientationObserver.canDetectOrientation()) {
			mOrientationObserver.enable();
		}
	}

	private class OrientationObserver extends OrientationEventListener {

		public OrientationObserver(Context context) {
			super(context);
		}

		@Override
		public void onOrientationChanged(int orientation) {
			orientation = (((orientation + 45) / 90) * 90) % 360;
			System.out.println("device orientation = " + orientation);
		}

	}

	private class RendererObserver implements QmCameraRender.Observer {
		@Override
		public void onSurfaceTextureCreated(SurfaceTexture surfaceTexture) {
			try {
				mQmCamera.mCamera.stopPreview();
				mQmCamera.setPreviewTexture(surfaceTexture);
				mQmCamera.mCamera.startPreview();
			} catch (final Exception ex) {
				LOGD("onSurfaceTextureCreated error: " + ex.getMessage());
			}
		}

		@Override
		public void onPhotoTextureCreated() {
		}

	}

	private void LOGD(String message) {
		if (DEBUG) {
			Log.i(TAG, message);
		}
	}

}