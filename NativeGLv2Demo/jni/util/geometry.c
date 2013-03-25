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
#include <string.h>
#include <stdio.h>
#include "CPUTMath.h"
#include "opengl.h"
#include "file.h"
#include "shader.h"
#include "Geometry.h"

Mesh* CreateTriangleMesh( void )
{
    // triangle vertices
    //
    //      0
    //     / \
    //    /   \
    //   /     \
    //  1-------2
    //
    const MeshVertex pTriangleVertices[] = 
    {
        // position     //      // normal     //      // tex coords //
        { {  0.0f,  1.0f,  0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.5f, 0.5f } },  // 0
        { { -1.0f, -1.0f,  0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },  // 1
        { {  1.0f, -1.0f,  0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },  // 2    
    };
    const unsigned int nTriangleVertexCount = 3;
    
    Mesh* pTriangle = ( Mesh* )malloc( sizeof( Mesh ) );
    pTriangle->pVertices = ( MeshVertex* )malloc( sizeof( MeshVertex ) * nTriangleVertexCount );
    memcpy( pTriangle->pVertices, pTriangleVertices, nTriangleVertexCount * sizeof( MeshVertex ) );
    pTriangle->nVertexCount = nTriangleVertexCount;
    
    return pTriangle;
}

Mesh* CreateQuadMesh( void )
{
    // quad vertices
    // 
    //  1-------0
    //  |       |
    //  |       |
    //  |       |
    //  2-------3
    //
    const MeshVertex pQuadVertices[] = 
    {
        // triangle #1: { 0, 1, 2 }
        { {  1.0f,  1.0f,  0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
        { { -1.0f,  1.0f,  0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
        { { -1.0f, -1.0f,  0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
        
        // triangle #2: { 0, 2, 3 }
        { {  1.0f,  1.0f,  0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
        { { -1.0f, -1.0f,  0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
        { {  1.0f, -1.0f,  0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
    };
    const unsigned int nQuadVertexCount = 6;
    
    Mesh* pQuad = ( Mesh* )malloc( sizeof( Mesh ) );
    pQuad->pVertices = ( MeshVertex* )malloc( sizeof( MeshVertex ) * nQuadVertexCount );
    memcpy( pQuad->pVertices, pQuadVertices, nQuadVertexCount * sizeof( MeshVertex ) );
    pQuad->nVertexCount = nQuadVertexCount;
    
    return pQuad;
}

Mesh* CreateCubeMesh( void )
{
    // cube vertices
    // 
    //
    //               9---------8
    //              /         /
    //             /         /
    //            10-------11
    //
    //    13         20-------21         4       front: {  0,  1,  2 }, {  0,  2,  3 }
    //    /|         |         |        /|       right: {  4,  5,  6 }, {  4,  6,  7 }
    //   / |         |         |       / |         top: {  8,  9, 10 }, {  8, 10, 11 }
    //  12 |      1---------0  |      5  |        left: { 12, 13, 14 }, { 12, 14, 15 }
    //  |  |      |  |      |  |      |  |      bottom: { 16, 17, 18 }, { 16, 18, 19 }
    //  | 14      |  23-----|-22      |  7        back: { 20, 21, 22 }, { 20, 22, 23 }
    //  | /       |         |         | /
    //  |/        |         |         |/
    //  15        2---------3         6
    //
    //               18-------19
    //              /         /
    //             /         /
    //            17-------16
    //
    const MeshVertex pCubeVertices[] = 
    {
        // FRONT
        // - triangle #1: { 0, 1, 2 }
        { {  1.0f,  1.0f,  1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
        { { -1.0f,  1.0f,  1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
        { { -1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
        
        // - triangle #2: { 0, 2, 3 }
        { {  1.0f,  1.0f,  1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
        { { -1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
        { {  1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
        
        // RIGHT
        // - triangle #1: { 4, 5, 6 }
        { {  1.0f,  1.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
        { {  1.0f,  1.0f,  1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },
        { {  1.0f, -1.0f,  1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
        
        // - triangle #2: { 4, 6, 7 }
        { {  1.0f,  1.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
        { {  1.0f, -1.0f,  1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
        { {  1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
        
        // TOP
        // - triangle #1: { 8, 9, 10 }
        { {  1.0f,  1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f } },
        { { -1.0f,  1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } },
        { { -1.0f,  1.0f,  1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f } },
        
        // - triangle #2: { 8, 10, 11 }
        { {  1.0f,  1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f } },
        { { -1.0f,  1.0f,  1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f } },
        { {  1.0f,  1.0f,  1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
        
        // LEFT
        // - triangle #1: { 12, 13, 14 }
        { { -1.0f,  1.0f,  1.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
        { { -1.0f,  1.0f, -1.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },
        { { -1.0f, -1.0f, -1.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
        
        // - triangle #2: { 12, 14, 15 }
        { { -1.0f,  1.0f,  1.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
        { { -1.0f, -1.0f, -1.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
        { { -1.0f, -1.0f,  1.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
        
        // BOTTOM
        // - triangle #1: { 16, 17, 18 }
        { {  1.0f, -1.0f,  1.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f } },
        { { -1.0f, -1.0f,  1.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 1.0f } },
        { { -1.0f, -1.0f, -1.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } },
        
        // - triangle #2: { 16, 18, 19 }
        { {  1.0f, -1.0f,  1.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f } },
        { { -1.0f, -1.0f, -1.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } },
        { {  1.0f, -1.0f, -1.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 0.0f } },
        
        // BACK
        // - triangle #1: { 20, 21, 22 }
        { { -1.0f,  1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f } },
        { {  1.0f,  1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f } },
        { {  1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } },
        
        // - triangle #2: { 20, 22, 23 }
        { { -1.0f,  1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f } },
        { {  1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } },
        { { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f } },
        
    };
    const unsigned int nCubeVertexCount = 36;
    
    Mesh* pCube = ( Mesh* )malloc( sizeof( Mesh ) );
    pCube->pVertices = ( MeshVertex* )malloc( sizeof( MeshVertex ) * nCubeVertexCount );
    memcpy( pCube->pVertices, pCubeVertices, nCubeVertexCount * sizeof( MeshVertex ) );
    pCube->nVertexCount = nCubeVertexCount;
    
    return pCube;
}

Mesh* CreateMesh( void* pVertices, unsigned int nVertexCount, void* pIndices, unsigned int nIndexCount )
{
    Mesh* pMesh;
    pMesh = ( Mesh* )malloc(sizeof( Mesh ) );
    
    pMesh->pVertices = ( MeshVertex* )malloc( sizeof( MeshVertex ) * nVertexCount );
    memcpy( pMesh->pVertices, pVertices, sizeof( MeshVertex ) * nVertexCount );
    pMesh->nVertexCount = nVertexCount;
    
    return pMesh;
}

