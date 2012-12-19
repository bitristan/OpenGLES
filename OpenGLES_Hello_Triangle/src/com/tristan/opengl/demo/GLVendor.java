package com.tristan.opengl.demo;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView;

public class GLVendor implements GLSurfaceView.Renderer {

	private float red = 0.9f;
	private float green = 0.2f;
	private float blue = 0.2f;

	private ShortBuffer indexBuffer;
	private FloatBuffer triangleBuffer;
	
	private short[] indexArray = {0, 1, 2};
	private int numberOfTriangles = 3;
	
	private void initTriangle() {
		ByteBuffer vbb = ByteBuffer.allocateDirect(numberOfTriangles * 3 * 4);
		vbb.order(ByteOrder.nativeOrder());
		triangleBuffer = vbb.asFloatBuffer();
		
		ByteBuffer ibb = ByteBuffer.allocateDirect(numberOfTriangles * 2);
		ibb.order(ByteOrder.nativeOrder());
		indexBuffer = ibb.asShortBuffer();
		
		float[] coordinates = {
			-0.5f, -0.5f, 0, 0.5f, -0.5f, 0f, 0f, 0.5f, 0f
		};
		
		triangleBuffer.put(coordinates);
		indexBuffer.put(indexArray);
		
		triangleBuffer.position(0);
		indexBuffer.position(0);
	}
	
	public void setColor(float red, float green, float blue) {
		this.red = red;
		this.green = green;
		this.blue = blue;
	}

	@Override
	public void onDrawFrame(GL10 gl) {
//		System.out.println("onDrawFrame");
//		gl.glClearColor(red, green, blue, 1.0f);
//		gl.glClear(GL10.GL_COLOR_BUFFER_BIT);
		
		gl.glClearColor(red, green, blue, 1.0f);
		gl.glClear(GL10.GL_COLOR_BUFFER_BIT);
		
		gl.glColor4f(0.5f, 0f, 0f, 0.5f);
		
		gl.glVertexPointer(3, GL10.GL_FLOAT, 0, triangleBuffer);
		
		gl.glDrawElements(GL10.GL_TRIANGLES, numberOfTriangles, GL10.GL_UNSIGNED_SHORT, indexBuffer);
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
//		gl.glViewport(0, 0, width, height);
		
		gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
		initTriangle();
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {

	}

}
