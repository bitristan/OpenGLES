/* Copyright (c) <2012>, Intel Corporation
 *
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice, 
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice, 
 *   this list of conditions and the following disclaimer in the documentation 
 *   and/or other materials provided with the distribution.
 * - Neither the name of Intel Corporation nor the names of its contributors 
 *   may be used to endorse or promote products derived from this software 
 *   without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */
 
#include <jni.h>
#include <sys/types.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "util/opengl.h"

extern "C" 
{
    #include "App.h"

    // Forward declare set asset manager.
    void SetAssetManager( AAssetManager* pManager );

    JNIEXPORT void JNICALL Java_com_android_particles_ParticlesLib_init( JNIEnv * env, jobject obj,  jint width, jint height );
    JNIEXPORT void JNICALL Java_com_android_particles_ParticlesLib_step( JNIEnv * env, jobject obj, jfloat fElapsedTime );
    JNIEXPORT void JNICALL Java_com_android_particles_ParticlesLib_createAssetManager( JNIEnv* env, jobject obj, jobject assetManager );
}

bool setupGraphics( int w, int h ) 
{
    // Init the app
    Init( w, h );

    return true;
}

void renderFrame( float fElapsedTime ) 
{
    // Update the app
    Update( fElapsedTime );
    
    // Render a frame from the app
    Render();
}

JNIEXPORT void JNICALL Java_com_android_particles_ParticlesLib_init( JNIEnv * env, jobject obj,  jint width, jint height )
{
    setupGraphics( width, height );
}

JNIEXPORT void JNICALL Java_com_android_particles_ParticlesLib_step( JNIEnv * env, jobject obj, jfloat fElapsedTime )
{
    renderFrame( fElapsedTime );
}

JNIEXPORT void JNICALL Java_com_android_particles_ParticlesLib_createAssetManager( JNIEnv* env, jobject obj, jobject assetManager )
{
    AAssetManager* mgr = AAssetManager_fromJava( env, assetManager );
    assert( mgr );
    
    // Store the assest manager for future use.
    SetAssetManager( mgr );
}

