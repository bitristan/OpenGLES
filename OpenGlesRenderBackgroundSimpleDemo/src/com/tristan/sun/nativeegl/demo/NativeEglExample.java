package com.tristan.sun.nativeegl.demo;

import android.app.Activity;
import android.os.Bundle;

public class NativeEglExample extends Activity {

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		setContentView(R.layout.main);

		drawBackgroud(1000, 1000);
	}

	public static native void drawBackgroud(int width, int height);

	static {
		System.loadLibrary("nativeegl");
	}

}
