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
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include "opengl.h"
#include "log.h"
#include "file.h"
#include "shader.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// Compilers the give shader type (vertex or fragment) and returns a handle
GLuint CompileShader( GLenum Type, const char* ShaderData, int Size )
{ 
    // Create the shader 
    GLuint ShaderHandle = glCreateShader( Type );

    if( ShaderHandle == 0 )
    {
        return 0;
    }

    // Set up string for #define based on Type 
    // Must keep the same length for the hard coded 17 for string size (see ShaderStringLengths)
    char* DefineVertexShader   = "#define VERTEX  \n";
    char* DefineFragmentShader = "#define FRAGMENT\n";
    char* ShaderDefine = ( Type == GL_VERTEX_SHADER ) ? DefineVertexShader : DefineFragmentShader;

    // Set up string table (first string is the #define for this Type and then the shader program)
    const char* ShaderStrings[2] = { ShaderDefine, ShaderData };
    GLint ShaderStringLengths[2] = { 17, Size };

    // Load the shader source
    glShaderSource( ShaderHandle, 2, ShaderStrings, ShaderStringLengths );
   
    // Compile the shader
    glCompileShader( ShaderHandle );

    // Check the compiler status
    GLint CompileStatus;
    glGetShaderiv( ShaderHandle, GL_COMPILE_STATUS, &CompileStatus );

    if( !CompileStatus ) 
    {
        GLint InfoLength = 0;

        glGetShaderiv( ShaderHandle, GL_INFO_LOG_LENGTH, &InfoLength );
      
        if( InfoLength > 1 )
        {
            char* InfoLog = (char*)malloc( sizeof(char) * InfoLength );

            glGetShaderInfoLog( ShaderHandle, InfoLength, NULL, InfoLog );
            
            char ErrorString[1024];
            sprintf( ErrorString, "Error lcompiling shader:\n%s\n", InfoLog ); 
            Log( ErrorString );
         
            free( InfoLog );
        }

        glDeleteShader( ShaderHandle );

        // Failed to compile the shadedrs
        assert( 0 );
        return 0;
    }

    return ShaderHandle;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Links the shaders and create the final program
GLuint LinkShader( GLuint VertexShaderHandle, GLuint FragmentShaderHandle )
{ 
    GLuint ProgramHandle = glCreateProgram();
    if( ProgramHandle == 0 )
    {
        // Failed to create a program handle
        assert( 0 );
        return 0;
    }

    // Associate the shaders with the program
    glAttachShader( ProgramHandle, VertexShaderHandle );
    glAttachShader( ProgramHandle, FragmentShaderHandle );

    // Link the program
    glLinkProgram( ProgramHandle );

    // Check the link status
    GLint  LinkerStatus;
    glGetProgramiv( ProgramHandle, GL_LINK_STATUS, &LinkerStatus );

    if( !LinkerStatus ) 
    {
        // Failed to link, get the error string
        GLint InfoLength = 0;

        glGetProgramiv( ProgramHandle, GL_INFO_LOG_LENGTH, &InfoLength );
      
        if( InfoLength > 1 )
        {
            char* InfoLog = (char*)malloc( sizeof(char) * InfoLength );

            glGetProgramInfoLog ( ProgramHandle, InfoLength, NULL, InfoLog );
            
            char ErrorString[1024];
            sprintf( ErrorString, "Error linking program:\n%s\n", InfoLog ); 
            Log( ErrorString );
         
            free( InfoLog );
        }

        glDeleteProgram( ProgramHandle );

        // Failed to link the shadedrs
        assert( 0 );
        return 0;
    }

    return ProgramHandle;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Loads, compiles, and links the shader for the given file and returns the programs id
GLuint LoadShader( const char* ShaderFileName )
{   
    GLuint ProgramHandle;

    // Open files
    char* pData = NULL;
    unsigned int Size = 0;
    
    ReadFile( ShaderFileName, &pData, &Size );

    // Load and compile the vertex shader
    GLuint VertexShader = CompileShader( GL_VERTEX_SHADER, pData, Size );
    if( VertexShader == 0 )
    {
        // Couldn't load the shader for the give file
        assert( 0 ); 
        return 0;
    }

    // Load and compiler the fragment shader
    GLuint FragmentShader = CompileShader( GL_FRAGMENT_SHADER, pData, Size );
    if( FragmentShader == 0 )
    {
        glDeleteShader( VertexShader );

        // Couldn't load the shader for the give file
        assert( 0 );
        return 0;
    }

    // Create the program handle
    ProgramHandle = LinkShader( VertexShader, FragmentShader );
    if( ProgramHandle == 0 )
    {
        // Failed to link the program
        assert( 0 );
        return 0;
    }

    // Free the shader resources
    glDeleteShader( VertexShader );
    glDeleteShader( FragmentShader );

    return ProgramHandle;
}

