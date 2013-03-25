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

#include <stdlib.h>

#include "CPUTMath.h"
#include "util/util.h"
#include "Particles.h"
#include "App.h"

unsigned int g_Width;
unsigned int g_Height;

float4x4 mWorld;
float4x4 mView;
float4x4 mProjection;

// particles
ParticleSystem* pParticles;

//--------------------------------------------------
// Initalize the app
// It's assumed that OpenGL is initialized and ready to do
//--------------------------------------------------
void Init( unsigned int width, unsigned int height )
{
    g_Width = width;
    g_Height = height;
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepthf(1.0f);
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    
    // init matrices
    mWorld = float4x4identity();
    mView = float4x4translation( 0.0f, -4.0f, -10.0f );

    float aspect = ( float )g_Width / ( float )g_Height;
    mProjection = float4x4PerspectiveFovRH( DegToRad( 65.0f ), aspect, 0.1f, 1000.0f );
    
    // particles
    pParticles = ( ParticleSystem* )malloc( sizeof( ParticleSystem ) );
    InitParticles( pParticles );
}

//--------------------------------------------------
// Update the frame
//--------------------------------------------------
void Update( float fElapsedTime )
{
    UpdateParticles( pParticles, fElapsedTime );
}

//--------------------------------------------------
// Render the frame
//--------------------------------------------------
void Render( void )
{
    glViewport( 0, 0, g_Width, g_Height );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // particles
    RenderParticles( pParticles, &mView, &mProjection );
}

// Shutdown the app and free all resources
void Shutdown( void )
{
    DestroyParticles( pParticles );
}
