package com.tristan.opengl.demo;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;

public class OpenGLDemo extends Activity {
	
	private GLView view;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        view = new GLView(this);
        setContentView(view);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    
}
