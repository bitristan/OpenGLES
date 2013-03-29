package com.tristan.sun.realtime.camera.demo;

import java.io.IOException;

import android.app.Activity;
import android.hardware.Camera;
import android.hardware.Camera.Size;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.Menu;
import android.view.SurfaceHolder;
import android.view.SurfaceHolder.Callback;
import android.view.SurfaceView;

public class CameraActivity extends Activity implements Callback {

	boolean hasSurface;
	SurfaceHolder surfaceHolder;
	Camera camera;

	SurfaceView surfaceView;
	GLSurfaceView glSurfaceView;
	CameraGLRender render;

	int width;
	int height;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_camera);

		surfaceView = (SurfaceView) findViewById(R.id.surfaceview);
		glSurfaceView = (GLSurfaceView) findViewById(R.id.glview);
		surfaceHolder = surfaceView.getHolder();
		render = new CameraGLRender(this, glSurfaceView);
		glSurfaceView.setEGLContextClientVersion(2);
		glSurfaceView.setRenderer(render);
		glSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
		glSurfaceView.setZOrderMediaOverlay(true);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.camera, menu);
		return true;
	}

	@SuppressWarnings("deprecation")
	@Override
	public void onResume() {
		super.onResume();

		if (hasSurface) {
			initCamera(surfaceHolder);
		} else {
			surfaceHolder.addCallback(this);
			surfaceHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
		}
	}

	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		System.out.println("surfaceCreated");
		if (!hasSurface) {
			hasSurface = true;
			initCamera(holder);
		}
	}

	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width,
			int height) {
		System.out.println("surfaceChanged, width = " + width + ";height = "
				+ height);
		camera.setPreviewCallback(render);
		camera.startPreview();
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		System.out.println("surfaceDestroyed");
		camera.setPreviewCallback(null);
		camera.stopPreview();
		camera.release();
		camera = null;
		hasSurface = false;
	}

	public void initCamera(SurfaceHolder surfaceHolder) {
		if (camera == null) {
			camera = Camera.open();

			try {
				camera.setDisplayOrientation(90);
				camera.setPreviewDisplay(null);
				Size size = camera.getParameters().getPreviewSize();
				width = size.width;
				height = size.height;
				render.setTextureSize(width, height);
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

}
