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

#include <assert.h>

#include "opengl.h"
#include "file.h"
#include "stb_image.h"
#include "texture.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
// Loads a PNG texture and returns a handle

GLuint LoadTexturePNG( const char* TextureFileName )
{   
    // Load Texture File
    char* pFileData = NULL;
    unsigned int fileSize = 0;
    
    ReadFile( TextureFileName, &pFileData, &fileSize );
    
    int width, height, numComponents;
    unsigned char* pData = stbi_load_from_memory( (unsigned char*)pFileData, fileSize, &width, &height, &numComponents, 0 );
    
    // Generate handle
    GLuint handle;
    glGenTextures( 1, &handle );
    
    // Bind the texture
    glBindTexture( GL_TEXTURE_2D, handle );
    
    // Set default filters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ); 
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    
    // Determine the format
    GLenum format;
    
    switch( numComponents )
    {
        case 1:
        {
            // Gray
            format = GL_LUMINANCE;
            break;
        }
        case 2: 
        {
            // Gray and Alpha
            format = GL_LUMINANCE_ALPHA;
            break;
        }
        case 3: 
        {
            // RGB
            format = GL_RGB;
            break;
        }
        case 4: 
        {
            // RGBA
            format = GL_RGBA;
            break;
        }
        default: 
        {
            // Unknown format
            assert(0);
            return 0;
        }
    }
    
    // Initialize the texture
    glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pData );
    
    glGenerateMipmap( GL_TEXTURE_2D );
    
    // Return handle
    return handle;  
}
