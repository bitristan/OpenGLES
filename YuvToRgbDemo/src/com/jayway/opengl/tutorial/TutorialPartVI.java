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

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.Bitmap.Config;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;

/**
 * This class is the setup for the Tutorial part VI located at:
 * http://blog.jayway.com/
 * 
 * @author Per-Erik Bergman (per-erik.bergman@jayway.com)
 * 
 */
public class TutorialPartVI extends Activity {
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		// Remove the title bar from the window.
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);

		// Make the windows into full screen mode.
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);

		setContentView(R.layout.main);

		// Create a OpenGL view.
		GLSurfaceView view = (GLSurfaceView) findViewById(R.id.surfaceView);
		view.setEGLContextClientVersion(2);

		// Creating and attaching the renderer.
		final OpenGLRenderer renderer = new OpenGLRenderer(this,
				readDataFromAssets("data.log"));
		view.setRenderer(renderer);
		view.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
	}

	public Data readDataFromAssets(String fileName) {
		int w = 960;
		int h = 720;
		byte[] data = new byte[w * h * 3 / 2];
		try {
			InputStream is = getAssets().open(fileName);
			is.read(data);
			is.close();

			return new Data(w, h, data);
		} catch (IOException e) {
			System.out.println("IoException:" + e.getMessage());
			e.printStackTrace();
		}

		return null;
	}

	public void checkYuvData() {
		int w = 960;
		int h = 720;
		byte[] data = new byte[w * h * 3 / 2];
		try {
			InputStream is = getAssets().open("data.log");
			is.read(data);
			is.close();
		} catch (IOException e) {
			System.out.println("IoException:" + e.getMessage());
			e.printStackTrace();
		}

		Data d = new Data(w, h, data);

		try {
			Bitmap bitmap = Bitmap.createBitmap(w, h, Config.ARGB_8888);
			bitmap.setPixels(d.rgb, 0, w, 0, 0, w, h);
			BufferedOutputStream bos = new BufferedOutputStream(
					new FileOutputStream(new File("/sdcard/test.jpg")));
			bitmap.compress(CompressFormat.JPEG, 100, bos);
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}

		System.out.println("bitmap success create...");
	}
}