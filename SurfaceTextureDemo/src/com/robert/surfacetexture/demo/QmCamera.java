package com.robert.surfacetexture.demo;

import java.io.IOException;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.hardware.Camera.CameraInfo;
import android.hardware.Camera.Parameters;

public class QmCamera {
	Camera mCamera;
	private int mCameraId;
	private final Camera.CameraInfo mCameraInfo = new Camera.CameraInfo();
	private SurfaceTexture mSurfaceTexture;

	public QmCamera(Context context) {
	}

	public boolean isCameraFront() {
		return mCameraInfo.facing == CameraInfo.CAMERA_FACING_FRONT;
	}

	/**
	 * Must be called from Activity.onPause(). Stops preview and releases Camera
	 * instance.
	 */
	public void onPause() {
		mSurfaceTexture = null;
		if (mCamera != null) {
			mCamera.stopPreview();
			mCamera.release();
			mCamera = null;
		}
	}

	/**
	 * Should be called from Activity.onResume(). Recreates Camera instance.
	 */
	public void onResume() {
		openCamera();
	}

	private void openCamera() {
		if (mCamera != null) {
			mCamera.stopPreview();
			mCamera.release();
			mCamera = null;
		}

		if (mCameraId >= 0) {
			Camera.getCameraInfo(mCameraId, mCameraInfo);
			mCamera = Camera.open(mCameraId);
			Parameters params = mCamera.getParameters();
			if (params.getSupportedFocusModes().contains(
					Parameters.FOCUS_MODE_CONTINUOUS_PICTURE)) {
				params.setFocusMode(Parameters.FOCUS_MODE_CONTINUOUS_PICTURE);
			}
			mCamera.setParameters(params);
			try {
				if (mSurfaceTexture != null) {
					mCamera.setPreviewTexture(mSurfaceTexture);
					mCamera.startPreview();
				}
			} catch (Exception ex) {
			}
		}
	}

	public void setCameraFront(boolean frontFacing) {
		int facing = frontFacing ? CameraInfo.CAMERA_FACING_FRONT
				: CameraInfo.CAMERA_FACING_BACK;

		mCameraId = -1;
		int numberOfCameras = Camera.getNumberOfCameras();
		for (int i = 0; i < numberOfCameras; ++i) {
			Camera.getCameraInfo(i, mCameraInfo);
			if (mCameraInfo.facing == facing) {
				mCameraId = i;
				break;
			}
		}

		openCamera();
	}

	public void setPreviewTexture(SurfaceTexture surfaceTexture)
			throws IOException {
		mSurfaceTexture = surfaceTexture;
		mCamera.setPreviewTexture(surfaceTexture);
	}

}
