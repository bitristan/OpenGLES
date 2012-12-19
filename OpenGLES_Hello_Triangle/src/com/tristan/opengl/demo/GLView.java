package com.tristan.opengl.demo;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.MotionEvent;

public class GLView extends GLSurfaceView {

	private GLVendor render;
	
	public GLView(Context context) {
		super(context);
		
		init(context);
	}

	public GLView(Context context, AttributeSet attrs) {
		super(context, attrs);
		
		init(context);
	}
	
	private void init(Context context) {
		render = new GLVendor();
		setRenderer(render);
	}

	@Override
	public boolean onTouchEvent(final MotionEvent event) {
//		queueEvent(new Runnable() {
//			
//			@Override
//			public void run() {
//				render.setColor(event.getX() / getWidth(), event.getY() / getHeight(), 1.0f);
//			}
//		});
		
		return true;
	}

}
