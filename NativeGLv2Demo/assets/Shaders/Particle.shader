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
 
#ifdef VERTEX
attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_texcoord;

uniform mat4 uWorldMatrix;
uniform mat4 uViewProjectionMatrix;
uniform float fLifeFactor;

varying vec3 vTransformedNormal;
varying vec2 vTexCoord;
varying float fLife;

void main()
{
    vTransformedNormal = vec3( uWorldMatrix * vec4( a_normal, 0.0 ) );
    vTexCoord = a_texcoord;
    fLife = fLifeFactor;

    gl_Position = uViewProjectionMatrix * vec4( a_position, 1.0 );
}
#endif


#ifdef FRAGMENT
precision mediump float;

uniform sampler2D diffuseSampler;

varying vec3 vTransformedNormal;
varying vec2 vTexCoord;
varying float fLife;

void main()
{
    // directional light
	vec3 vLightDir = normalize( vec3( -1.0, 1.0, 1.0 ) );
    vec3 vNormal = normalize( vTransformedNormal );
	float NdotL = max( dot( vLightDir, vNormal ), 0.0 );
    
    // diffuse
    vec4 vDiffuseColor = vec4( 1, 1.0 - fLife, 0, 1 ); 
    vec4 vTexColor = texture2D( diffuseSampler, vTexCoord.st );
    vDiffuseColor *= vTexColor;
    vDiffuseColor.rgb *= NdotL;
    
    // final color
    vec4 vColor = vec4( 0.0, 0.0, 0.0, 1.0 );
    vColor.rgb = clamp( vDiffuseColor.rgb, 0.0, 1.0 );
    vColor.a = vDiffuseColor.a * fLife;
    
    // save it out
    gl_FragColor = vColor;
}
#endif
