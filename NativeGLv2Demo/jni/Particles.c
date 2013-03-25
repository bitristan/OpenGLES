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

//--------------------------------------------------
// Init particle system
//--------------------------------------------------
void InitParticles( ParticleSystem* pParticleSystem )
{
    // allocate a chunk of memory to be used by the particle system
    // the chunk is defined by MAX_PARTICLES
    pParticleSystem->pParticles = ( Particle* )malloc( sizeof(Particle) * MAX_PARTICLES );
    
    // the emitter starts at (0, 0, 0)
    pParticleSystem->vSourcePosition = float3zero();
    
    // load the shader and tie the attributes and uniforms
    pParticleSystem->nShaderProgram = LoadShader( "Shaders/Particle.shader" );
    
    // define the type of mesh to use for the particles
    pParticleSystem->pParticleMesh = CreateQuadMesh();

    // Get attirbute locations
    pParticleSystem->pParticleMesh->nPositionAttrib = glGetAttribLocation ( pParticleSystem->nShaderProgram, "a_position" );
    pParticleSystem->pParticleMesh->nNormalAttrib = glGetAttribLocation( pParticleSystem->nShaderProgram, "a_normal" );
    pParticleSystem->pParticleMesh->nTextCoordAttrib = glGetAttribLocation( pParticleSystem->nShaderProgram, "a_texcoord" );
    
    // Get uniform locations
    pParticleSystem->nWorldUniform = glGetUniformLocation( pParticleSystem->nShaderProgram, "uWorldMatrix" );
    pParticleSystem->nViewProjectionUniform = glGetUniformLocation( pParticleSystem->nShaderProgram, "uViewProjectionMatrix" );
    pParticleSystem->nLife = glGetUniformLocation( pParticleSystem->nShaderProgram, "fLifeFactor" );
    
    // texture
    pParticleSystem->nDiffuseTexture = LoadTexturePNG( "art/particle.png" );
    pParticleSystem->nDiffuseTextureLoc = glGetUniformLocation( pParticleSystem->nShaderProgram, "diffuseSampler" );
    
    // init particles
    // NOTE: in this case the max number of particles are initialized
    float fLowestSpeed = 1.0f;
    float fHighestSpeed = 4.0f;
    float fRangeSpeed = ( fHighestSpeed - fLowestSpeed ) + 1;
    unsigned int ii = 0;
    for( ii = 0; ii < MAX_PARTICLES; ++ii )
    {
        Particle* pParticle = &pParticleSystem->pParticles[ ii ];
        
        pParticle->vPos = pParticleSystem->vSourcePosition;
        pParticle->fLife = -1.0f;
        pParticle->mTransform = float4x4translationVector( &pParticle->vPos );
        fLowestSpeed = -2.0;
        fHighestSpeed = 2.0f;
        fRangeSpeed = ( fHighestSpeed - fLowestSpeed ) + 1;
        float f = ( float )( fLowestSpeed + ( fRangeSpeed * rand() / (RAND_MAX + 1.0f) ) );
        pParticle->vVel.x = f;
        fLowestSpeed = 4.0;
        fHighestSpeed = 8.0f;
        fRangeSpeed = ( fHighestSpeed - fLowestSpeed ) + 1;
        f = ( float )( fLowestSpeed + ( fRangeSpeed * rand() / (RAND_MAX + 1.0f) ) );
        pParticle->vVel.y = f;
    } 
}

//--------------------------------------------------
// Clean up memory allocations
//--------------------------------------------------
void DestroyParticles( ParticleSystem* pParticleSystem )
{
    if( pParticleSystem != NULL )
    {
        // free up all the memory that was allocated on init
        free( pParticleSystem->pParticleMesh );
        free( pParticleSystem->pParticles );
        free( pParticleSystem );
    }
}

//--------------------------------------------------
// Update the particles
//--------------------------------------------------
void UpdateParticles( ParticleSystem* pParticleSystem, float fElapsedTime )
{
    static float fRotation = 0.0f;
    fRotation += fElapsedTime;
    int spawn = 0;
    unsigned ii = 0;
    for( ii = 0; ii < MAX_PARTICLES; ++ii )
    {
        Particle* pParticle = &pParticleSystem->pParticles[ ii ];
        
        // undead
        if(pParticle->fLife > 0.0f)
        {
            pParticle->mTransform = float4x4translationVector(&pParticle->vPos);

            unsigned int bIsEven = ( ( ii % 2 ) == 0 ) ? 1 : 0;
            float4x4 mRotateZ = float4x4RotationZ( ( bIsEven ) ? fRotation : -fRotation );
            pParticle->mTransform = float4x4multiply( &mRotateZ, &pParticle->mTransform );

            float3 vel = float3multiplyScalar( &pParticle->vVel, fElapsedTime );
            pParticle->vPos = float3add( &pParticle->vPos, &vel );

            pParticle->fLife -= pParticle->vVel.y * fElapsedTime;
        }
        // dead
        else
        {
            // re-spawn a max of 4 particles every frame
            if( spawn++ > 4 )
                continue;
            pParticle->vPos = pParticleSystem->vSourcePosition;
            pParticle->fLife = MAX_LIFE;
            pParticle->mTransform = float4x4translationVector( &pParticle->vPos );
        }

        float fScaleFactor = pParticle->vPos.y * 0.25f;
        float4x4 mScale = float4x4Scale( fScaleFactor, fScaleFactor, fScaleFactor );
        pParticle->mTransform = float4x4multiply( &mScale, &pParticle->mTransform );
    }
}

//--------------------------------------------------
// Render the particles
//--------------------------------------------------
void RenderParticles( ParticleSystem* pParticleSystem, float4x4* mView, float4x4* mProj )
{
    // Set the shader program
    glUseProgram( pParticleSystem->nShaderProgram );
    
    // all the particles are using the same texture so it only 
    // needs to be set once for all the particles
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, pParticleSystem->nDiffuseTexture );
    glUniform1i( pParticleSystem->nDiffuseTextureLoc, 0 );
    
    // loop through the particles
    unsigned int ii = 0;
    for( ii = 0; ii < MAX_PARTICLES; ++ii )
    {
        // current particle
        Particle* pParticle = &pParticleSystem->pParticles[ ii ];
        // pointer to the particle mesh
        Mesh* pMesh = pParticleSystem->pParticleMesh;
        
        // only draw the particle if it is alive
        if( pParticle->fLife > 0.0f )
        {
            // set the particle transform uniform
            glUniformMatrix4fv( pParticleSystem->nWorldUniform, 1, GL_FALSE, ( const GLfloat* )&pParticle->mTransform );
            
            // set view and projection matrices
            float4x4 mViewProj = float4x4multiply( &pParticle->mTransform, mView );
            mViewProj = float4x4multiply( &mViewProj, mProj );
            glUniformMatrix4fv( pParticleSystem->nViewProjectionUniform, 1, GL_FALSE, ( const GLfloat* )&mViewProj );
            
            // pass in the remaining life to be used as the opacity of the particle
            glUniform1f( pParticleSystem->nLife, pParticle->fLife / MAX_LIFE );
            
            // set the vertex attributes (position, normal, texture coords)
            glEnableVertexAttribArray( pMesh->nPositionAttrib );
            glEnableVertexAttribArray( pMesh->nNormalAttrib );
            glEnableVertexAttribArray( pMesh->nTextCoordAttrib );
            glVertexAttribPointer( pMesh->nPositionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof( MeshVertex ), &pMesh->pVertices->vPos );
            glVertexAttribPointer( pMesh->nNormalAttrib, 3, GL_FLOAT, GL_FALSE, sizeof( MeshVertex ), &pMesh->pVertices->vNormal );
            glVertexAttribPointer( pMesh->nTextCoordAttrib, 2, GL_FLOAT, GL_FALSE, sizeof( MeshVertex ), &pMesh->pVertices->vTexCoord );
            
            // Draw
            glDrawArrays( GL_TRIANGLES, 0, pMesh->nVertexCount );
        }
    }
    
    // unbind the particle texture
    glBindTexture( GL_TEXTURE_2D, 0 );
}
