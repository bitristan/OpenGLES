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
 
#pragma once
#ifndef __PARTICLES_H__
#define __PARTICLES_H__

// define some MAX values
static const unsigned int MAX_PARTICLES = 128;
static const float MAX_LIFE = 10.0f;

// define the particle properties
typedef struct _Particle
{
    // current particle position
    float3 vPos;
    // current particle velocity
    float3 vVel;
    // remaining particle life
    // used to determine opacity
    float fLife;
    // transform matrix sent to the shader
    float4x4 mTransform;
} Particle;

// define the particle system properties
typedef struct _ParticleSystem
{
    // array of particles
    Particle* pParticles;
    
    // particle emitter position in the world
    float3 vSourcePosition;
    
    // GL defines //

    // the shaders
    GLuint nShaderProgram;
    // holds the particle transform matrix
    GLint  nWorldUniform;
    // holds the view projection matrix for the camera/eye
    GLint  nViewProjectionUniform;
    
    // holds the particle's remaining life
    GLint nLife;

    // texture uniforms
    GLuint nDiffuseTexture;
    GLuint nDiffuseTextureLoc;
    
    // vertices used to draw the particle
    Mesh* pParticleMesh;
} ParticleSystem;


// Particle system functions //

// create a particle system
// this function allocates memory that should be freed later
void InitParticles( ParticleSystem* pParticleSystem );
// free allocated memory assigned to a particle system
void DestroyParticles( ParticleSystem* pParticleSystem );
// particle simulation step
// this is where particle behavior is defined
void UpdateParticles( ParticleSystem* pParticleSystem, float fElapsedTime );
// draw particles on screen
void RenderParticles( ParticleSystem* pParticleSystem, float4x4* mView, float4x4* mProj );


#endif // __PARTICLES_H__
