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

#ifndef __CPUTMath_h__
#define __CPUTMath_h__
#include <math.h>

/*
 * Constants
 */
static const float kEpsilon  = 0.00001f;
static const float kPi       = 3.141592653589793238462643383280f;
static const float k2Pi      = 6.283185307179586476925286766559f;
static const float kPiDiv2   = 1.570796326794896619231321691640f;
static const float kInvPi    = 0.318309886183790671537767526745f;
static const float kDegToRad = 0.017453292519943295769236907685f;
static const float kRadToDeg = 57.29577951308232087679815481410f;
static float DegToRad( float fDeg ) { return fDeg * kDegToRad; }
static float RadToDeg( float fRad ) { return fRad * kRadToDeg; }

#define swapf(a, b) { float t = a; a = b; b = t; }

typedef struct float2 float2;
typedef struct float3 float3;
typedef struct float4 float4;
typedef struct float3x3 float3x3;
typedef struct float4x4 float4x4;
typedef float4 quaternion;

/*
 * Struct definition
 */ 
struct float2
{
    float x;
    float y;
};
struct float3
{
    float x;
    float y;
    float z;
};
struct float4
{
    float x;
    float y;
    float z;
    float w;
};
struct float3x3
{
    float3 r0;
    float3 r1;
    float3 r2;
};
struct float4x4
{
    float4 r0;
    float4 r1;
    float4 r2;
    float4 r3;
};

/*
 * float3 functions
 */
static float3 float3zero(void)
{
    float3 zero = { 0,0,0 };
    return zero;
}

/* Vector operations */
static float3 float3add(const float3* a, const float3* b)
{
    float3 r = { a->x + b->x, a->y + b->y, a->z + b->z };
    return r;
}
static float3 float3subtract(const float3* a, const float3* b)
{
    float3 r = { a->x - b->x, a->y - b->y, a->z - b->z  };
    return r;
}
static float3 float3multiply(const float3* a, const float3* b)
{
    float3 r = { a->x * b->x, a->y * b->y, a->z * b->z  };
    return r;
}
static float3 float3divide(const float3* a, const float3* b)
{
    float3 r = { a->x / b->x, a->y / b->y, a->z / b->z  };
    return r;
}

/* Scalar operations */
static float3 float3addScalar(const float3* v, float f)
{
    float3 r = { v->x+f, v->y+f, v->z+f };
    return r;
}
static float3 float3subtractScalar(const float3* v, float f)
{
    float3 r = { v->x-f, v->y-f, v->z-f };
    return r;
}
static float3 float3multiplyScalar(const float3* v, float f)
{
    float3 r = { v->x*f, v->y*f, v->z*f };
    return r;
}
static float3 float3divideScalar(const float3* v, float f)
{
    float3 r = { v->x/f, v->y/f, v->z/f };
    return r;
}

/* Misc */
static int float3equal(const float3* a, const float3* b)
{
    return  (a->x == b->x) &&
            (a->y == b->y) &&
            (a->z == b->z);
}

static float float3hadd(const float3* v)
{
    return v->x + v->y + v->z;
}

static float float3lengthSq(const float3* v)
{
    return v->x*v->x + v->y*v->y + v->z*v->z;
}
static float float3length(const float3* v) 
{
    return sqrtf(float3lengthSq(v));
}
static float3 float3normalize(const float3* v)
{
    return float3divideScalar(v, float3length(v));
}
static float float3dot(const float3* l, const float3* r)
{
    return l->x*r->x + l->y*r->y + l->z*r->z;
}
static float3 float3cross(const float3* l, const float3* r)
{
    float3 res = {  l->y*r->z - l->z*r->y,
                    l->z*r->x - l->x*r->z,
                    l->x*r->y - l->y*r->x };
    return res;
}

/*
 * float4 functions
 */
static float4 float4zero(void)
{
    float4 zero = { 0,0,0,0 };
    return zero;
}

/* Vector operations */
static float4 float4add(const float4* a, const float4* b)
{
    float4 r = { a->x + b->x, a->y + b->y, a->z + b->z, a->w + b->w };
    return r;
}
static float4 float4subtract(const float4* a, const float4* b)
{
    float4 r = { a->x - b->x, a->y - b->y, a->z - b->z, a->w - b->w  };
    return r;
}
static float4 float4multiply(const float4* a, const float4* b)
{
    float4 r = { a->x * b->x, a->y * b->y, a->z * b->z, a->w * b->w  };
    return r;
}
static float4 float4divide(const float4* a, const float4* b)
{
    float4 r = { a->x / b->x, a->y / b->y, a->z / b->z, a->w / b->w  };
    return r;
}

/* Scalar operations */
static float4 float4addScalar(const float4* v, float f)
{
    float4 r = { v->x+f, v->y+f, v->z+f, v->w+f };
    return r;
}
static float4 float4subtractScalar(const float4* v, float f)
{
    float4 r = { v->x-f, v->y-f, v->z-f, v->w-f };
    return r;
}
static float4 float4multiplyScalar(const float4* v, float f)
{
    float4 r = { v->x*f, v->y*f, v->z*f, v->w*f };
    return r;
}
static float4 float4divideScalar(const float4* v, float f)
{
    float4 r = { v->x/f, v->y/f, v->z/f, v->w/f };
    return r;
}

/* Misc */
static int float4equal(const float4* a, const float4* b)
{
    return  (a->x == b->x) &&
            (a->y == b->y) &&
            (a->z == b->z) &&
            (a->w == b->w);
}

static float float4hadd(const float4* v)
{
    return v->x + v->y + v->z + v->w;
}

static float float4lengthSq(const float4* v)
{
    return (v->x*v->x + v->y*v->y + v->z*v->z + v->w*v->w);
}
static float float4length(const float4* v) 
{
    return sqrtf(float4lengthSq(v));
}
static float4 float4normalize(const float4* v)
{
    return float4divideScalar(v, float4length(v));
}
static float float4dot(const float4* l, const float4* r)
{
    return l->x*r->x + l->y*r->y + l->z*r->z + l->w*r->w;
}

/*
 * float3x3
 */

/* Matrix operations */
static float3x3 float3x3identity(void)
{
    float3x3 identity =
    {
        {1,0,0},
        {0,1,0},
        {0,0,1},
    };

    return identity;
}

#define MTX3_INDEX(f,r,c) ((f)[(r*3)+c])
static float3x3 float3x3multiply(const float3x3* a, const float3x3* b)
{
    float3x3 m = float3x3identity();

    const float* left     = (const float*)a;
    const float* right    = (const float*)b;
    float* result   = (float*)&m;

    int ii, jj, kk;
    for(ii=0; ii<3; ++ii) /* row */
    {
        for(jj=0; jj<3; ++jj) /* column */
        {
            float sum = MTX3_INDEX(left,ii,0)*MTX3_INDEX(right,0,jj);
            for(kk=1; kk<3; ++kk)
            {
                sum += (MTX3_INDEX(left,ii,kk)*MTX3_INDEX(right,kk,jj));
            }
            MTX3_INDEX(result,ii,jj) = sum;
        }
    }
    return m;
}
#undef MTX3_INDEX

static float3 float3x3multiplyFloat3(const float3x3* m, const float3* v)
{
    float3 res;
    float3 t;

    t = float3multiply(&m->r0, v);
    res.x = float3hadd(&t);

    t = float3multiply(&m->r1, v);
    res.y = float3hadd(&t);

    t = float3multiply(&m->r2, v);
    res.z = float3hadd(&t);

    return res;
}

static float3x3 float3x3addScalar(const float3x3* m, float f)
{
    float3x3 result;
    result.r0 = float3addScalar(&m->r0, f);
    result.r1 = float3addScalar(&m->r1, f);
    result.r2 = float3addScalar(&m->r2, f);
    return result;
}
static float3x3 float3x3subtractScalar(const float3x3* m, float f)
{
    float3x3 result;
    result.r0 = float3subtractScalar(&m->r0, f);
    result.r1 = float3subtractScalar(&m->r1, f);
    result.r2 = float3subtractScalar(&m->r2, f);
    return result;
}
static float3x3 float3x3multiplyScalar(const float3x3* m, float f)
{
    float3x3 result;
    result.r0 = float3multiplyScalar(&m->r0, f);
    result.r1 = float3multiplyScalar(&m->r1, f);
    result.r2 = float3multiplyScalar(&m->r2, f);
    return result;
}
static float3x3 float3x3divideScalar(const float3x3* m, float f)
{
    float3x3 result;
    result.r0 = float3divideScalar(&m->r0, f);
    result.r1 = float3divideScalar(&m->r1, f);
    result.r2 = float3divideScalar(&m->r2, f);
    return result;
}

/* Other math */
static int float3x3equal(const float3x3* l, const float3x3* r)
{
    return  float3equal(&l->r0, &r->r0) &&
            float3equal(&l->r1, &r->r1) &&
            float3equal(&l->r2, &r->r2);
}

static float float3x3determinant(const float3x3* m)
{
    float f0 = m->r0.x *  (m->r1.y*m->r2.z - m->r2.y*m->r1.z);
    float f1 = m->r0.y * -(m->r1.x*m->r2.z - m->r2.x*m->r1.z);
    float f2 = m->r0.z *  (m->r1.x*m->r2.y - m->r2.x*m->r1.y);

    return f0 + f1 + f2;
}
static float3x3 float3x3transpose(const float3x3* m)
{
    float3x3 result = *m;
    swapf(result.r0.y, result.r1.x);
    swapf(result.r0.z, result.r2.x);
    swapf(result.r1.z, result.r2.y);
    return result;
}

static float3x3 float3x3inverse(const float3x3* m)
{
    float det = float3x3determinant(m);
    float3x3 inv;

    inv.r0.x =   (m->r1.y*m->r2.z) - (m->r1.z*m->r2.y);
    inv.r0.y = -((m->r1.x*m->r2.z) - (m->r1.z*m->r2.x));
    inv.r0.z =   (m->r1.x*m->r2.y) - (m->r1.y*m->r2.x);

    inv.r1.x = -((m->r0.y*m->r2.z) - (m->r0.z*m->r2.y));
    inv.r1.y =   (m->r0.x*m->r2.z) - (m->r0.z*m->r2.x);
    inv.r1.z = -((m->r0.x*m->r2.y) - (m->r0.y*m->r2.x));

    inv.r2.x =   (m->r0.y*m->r1.z) - (m->r0.z*m->r1.y);
    inv.r2.y = -((m->r0.x*m->r1.z) - (m->r0.z*m->r1.x));
    inv.r2.z =   (m->r0.x*m->r1.y) - (m->r0.y*m->r1.x);

    inv = float3x3transpose(&inv);
    inv = float3x3divideScalar(&inv, det);

    return inv;
}
/* Matrix construction */
static float3x3 float3x3RotationX(float rad)
{
    float       c = cosf( rad );
    float       s = sinf( rad );
    float3x3    m = { { 1.0f, 0.0f, 0.0f},
                      { 0.0f,    c,    s},
                      { 0.0f,   -s,    c} };
    return m;
}
static float3x3 float3x3RotationY(float rad)
{
    float       c = cosf( rad );
    float       s = sinf( rad );
    float3x3    m = {  c, 0.0f,   -s,
                    0.0f, 1.0f, 0.0f,
                       s, 0.0f,    c };
    return m;
}
static float3x3 float3x3RotationZ(float rad)
{

    float       c = cosf( rad );
    float       s = sinf( rad );
    float3x3    m = {  c,    s, 0.0f,
                      -s,    c, 0.0f,
                    0.0f, 0.0f, 1.0f };
    return m;
}

static float3x3 float3x3RotationAxis(const float3* axis, float rad )
{
    float3 normAxis = float3normalize(axis);
    float c = cosf(rad);
    float s = sinf(rad);
    float t = 1 - c;

    float x = normAxis.x;
    float y = normAxis.y;
    float z = normAxis.z;

    float3x3 m;

    m.r0.x = (t * x * x) + c;
    m.r0.y = (t * x * y) + s * z;
    m.r0.z = (t * x * z) - s * y;

    m.r1.x = (t * x * y) - (s * z);
    m.r1.y = (t * y * y) + c;
    m.r1.z = (t * y * z) + (s * x);

    m.r2.x = (t * x * z) + (s * y);
    m.r2.y = (t * y * z) - (s * x);
    m.r2.z = (t * z * z) + c;

    return m;
}

static float3x3 float3x3Scale(float x, float y, float z)
{
    float3x3 m = {x,0,0,
                  0,y,0,
                  0,0,z};
    return m;
}


/*
 * float4x4
 */

/* Matrix operations */
static float4x4 float4x4identity(void)
{
    float4x4 identity =
    {
        {1,0,0,0},
        {0,1,0,0},
        {0,0,1,0},
        {0,0,0,1}
    };

    return identity;
}

static float4x4 float4x4fromFloat3x3(const float3x3* m)
{
    float4x4 r =
    {
        { m->r0.x, m->r0.y, m->r0.z, 0.0f },
        { m->r1.x, m->r1.y, m->r1.z, 0.0f },
        { m->r2.x, m->r2.y, m->r2.z, 0.0f },
        {    0.0f,    0.0f,    0.0f, 1.0f },
    };

    return r;
}

#define MTX4_INDEX(f,r,c) ((f)[(r*4)+c])
static float4x4 float4x4multiply(const float4x4* a, const float4x4* b)
{

    float4x4 m = float4x4identity();

    const float* left     = (const float*)a;
    const float* right    = (const float*)b;
    float* result   = (float*)&m;

    int ii, jj, kk;
    for(ii=0; ii<4; ++ii) /* row */
    {
        for(jj=0; jj<4; ++jj) /* column */
        {
            float sum = MTX4_INDEX(left,ii,0)*MTX4_INDEX(right,0,jj);
            for(kk=1; kk<4; ++kk)
            {
                sum += (MTX4_INDEX(left,ii,kk)*MTX4_INDEX(right,kk,jj));
            }
            MTX4_INDEX(result,ii,jj) = sum;
        }
    }
    return m;
}
#undef MTX4_INDEX

static float4 float4x4multiplyfloat4(const float4x4* m, const float4* v)
{
    float4 res;
    float4 t;

    t = float4multiply(&m->r0, v);
    res.x = float4hadd(&t);

    t = float4multiply(&m->r1, v);
    res.y = float4hadd(&t);

    t = float4multiply(&m->r2, v);
    res.z = float4hadd(&t);

    t = float4multiply(&m->r3, v);
    res.w = float4hadd(&t);

    return res;
}

static float4x4 float4x4addScalar(const float4x4* m, float f)
{
    float4x4 result;
    result.r0 = float4addScalar(&m->r0, f);
    result.r1 = float4addScalar(&m->r1, f);
    result.r2 = float4addScalar(&m->r2, f);
    result.r3 = float4addScalar(&m->r3, f);
    return result;
}
static float4x4 float4x4subtractScalar(const float4x4* m, float f)
{
    float4x4 result;
    result.r0 = float4subtractScalar(&m->r0, f);
    result.r1 = float4subtractScalar(&m->r1, f);
    result.r2 = float4subtractScalar(&m->r2, f);
    result.r3 = float4subtractScalar(&m->r3, f);
    return result;
}
static float4x4 float4x4multiplyScalar(const float4x4* m, float f)
{
    float4x4 result;
    result.r0 = float4multiplyScalar(&m->r0, f);
    result.r1 = float4multiplyScalar(&m->r1, f);
    result.r2 = float4multiplyScalar(&m->r2, f);
    result.r3 = float4multiplyScalar(&m->r3, f);
    return result;
}
static float4x4 float4x4divideScalar(const float4x4* m, float f)
{
    float4x4 result;
    result.r0 = float4divideScalar(&m->r0, f);
    result.r1 = float4divideScalar(&m->r1, f);
    result.r2 = float4divideScalar(&m->r2, f);
    result.r3 = float4divideScalar(&m->r3, f);
    return result;
}

/* Other math */
static int float4x4equal(const float4x4* l, const float4x4* r)
{
    return  float4equal(&l->r0, &r->r0) &&
            float4equal(&l->r1, &r->r1) &&
            float4equal(&l->r2, &r->r2) &&
            float4equal(&l->r3, &r->r3);
}

static float float4x4determinant(const float4x4* m)
{
    float det = 0.0f;

    float3x3 a = {  m->r1.y,m->r1.z,m->r1.w,
                    m->r2.y,m->r2.z,m->r2.w,
                    m->r3.y,m->r3.z,m->r3.w};

    float3x3 b = {  m->r1.x,m->r1.z,m->r1.w,
                    m->r2.x,m->r2.z,m->r2.w,
                    m->r3.x,m->r3.z,m->r3.w};

    float3x3 c = {  m->r1.x,m->r1.y,m->r1.w,
                    m->r2.x,m->r2.y,m->r2.w,
                    m->r3.x,m->r3.y,m->r3.w};

    float3x3 d = {  m->r1.x,m->r1.y,m->r1.z,
                    m->r2.x,m->r2.y,m->r2.z,
                    m->r3.x,m->r3.y,m->r3.z};


    det += m->r0.x * float3x3determinant(&a);

    det -= m->r0.y * float3x3determinant(&b);

    det += m->r0.z * float3x3determinant(&c);

    det -= m->r0.w * float3x3determinant(&d);

    return det;
}
static float4x4 float4x4transpose(const float4x4* m)
{
    float4x4 result = *m;
    swapf(result.r0.y, result.r1.x);
    swapf(result.r0.z, result.r2.x);
    swapf(result.r0.w, result.r3.x);
    swapf(result.r1.z, result.r2.y);
    swapf(result.r1.w, result.r3.y);
    swapf(result.r2.w, result.r3.z);
    return result;
}

static float4x4 float4x4inverse(const float4x4* mat)
{
    float4x4 ret;
    float recip;

    /* temp matrices */

    /* row 1 */
    float3x3 a = { mat->r1.y,mat->r1.z,mat->r1.w,
        mat->r2.y,mat->r2.z,mat->r2.w,
        mat->r3.y,mat->r3.z,mat->r3.w };

    float3x3 b = { mat->r1.x,mat->r1.z,mat->r1.w,
        mat->r2.x,mat->r2.z,mat->r2.w,
        mat->r3.x,mat->r3.z,mat->r3.w };

    float3x3 c = { mat->r1.x,mat->r1.y,mat->r1.w,
        mat->r2.x,mat->r2.y,mat->r2.w,
        mat->r3.x,mat->r3.y,mat->r3.w };

    float3x3 d = { mat->r1.x,mat->r1.y,mat->r1.z,
        mat->r2.x,mat->r2.y,mat->r2.z,
        mat->r3.x,mat->r3.y,mat->r3.z };

    /* row 2 */
    float3x3 e = { mat->r0.y,mat->r0.z,mat->r0.w,
        mat->r2.y,mat->r2.z,mat->r2.w,
        mat->r3.y,mat->r3.z,mat->r3.w };

    float3x3 f = { mat->r0.x,mat->r0.z,mat->r0.w,
        mat->r2.x,mat->r2.z,mat->r2.w,
        mat->r3.x,mat->r3.z,mat->r3.w };

    float3x3 g = { mat->r0.x,mat->r0.y,mat->r0.w,
        mat->r2.x,mat->r2.y,mat->r2.w,
        mat->r3.x,mat->r3.y,mat->r3.w };

    float3x3 h = { mat->r0.x,mat->r0.y,mat->r0.z,
        mat->r2.x,mat->r2.y,mat->r2.z,
        mat->r3.x,mat->r3.y,mat->r3.z };


    /* row 3 */
    float3x3 i = { mat->r0.y,mat->r0.z,mat->r0.w,
        mat->r1.y,mat->r1.z,mat->r1.w,
        mat->r3.y,mat->r3.z,mat->r3.w };

    float3x3 j = { mat->r0.x,mat->r0.z,mat->r0.w,
        mat->r1.x,mat->r1.z,mat->r1.w,
        mat->r3.x,mat->r3.z,mat->r3.w };

    float3x3 k = { mat->r0.x,mat->r0.y,mat->r0.w,
        mat->r1.x,mat->r1.y,mat->r1.w,
        mat->r3.x,mat->r3.y,mat->r3.w };

    float3x3 l = { mat->r0.x,mat->r0.y,mat->r0.z,
        mat->r1.x,mat->r1.y,mat->r1.z,
        mat->r3.x,mat->r3.y,mat->r3.z };


    /* row 4 */
    float3x3 m = { mat->r0.y, mat->r0.z, mat->r0.w,
        mat->r1.y, mat->r1.z, mat->r1.w,
        mat->r2.y, mat->r2.z, mat->r2.w };

    float3x3 n = { mat->r0.x, mat->r0.z, mat->r0.w,
        mat->r1.x, mat->r1.z, mat->r1.w,
        mat->r2.x, mat->r2.z, mat->r2.w };

    float3x3 o = { mat->r0.x,mat->r0.y,mat->r0.w,
        mat->r1.x,mat->r1.y,mat->r1.w,
        mat->r2.x,mat->r2.y,mat->r2.w };

    float3x3 p = { mat->r0.x,mat->r0.y,mat->r0.z,
        mat->r1.x,mat->r1.y,mat->r1.z,
        mat->r2.x,mat->r2.y,mat->r2.z };

    /* row 1 */
    ret.r0.x = float3x3determinant(&a);

    ret.r0.y = -float3x3determinant(&b);

    ret.r0.z = float3x3determinant(&c);

    ret.r0.w = -float3x3determinant(&d);

    /* row 2 */
    ret.r1.x = -float3x3determinant(&e);

    ret.r1.y = float3x3determinant(&f);

    ret.r1.z = -float3x3determinant(&g);

    ret.r1.w = float3x3determinant(&h);

    /* row 3 */
    ret.r2.x = float3x3determinant(&i);

    ret.r2.y = -float3x3determinant(&j);

    ret.r2.z = float3x3determinant(&k);

    ret.r2.w = -float3x3determinant(&l);

    /* row 4 */
    ret.r3.x = -float3x3determinant(&m);

    ret.r3.y = float3x3determinant(&n);

    ret.r3.z = -float3x3determinant(&o);

    ret.r3.w = float3x3determinant(&p);

    ret = float4x4transpose(&ret);
    recip = 1.0f/float4x4determinant(mat);
    ret = float4x4multiplyScalar(&ret, recip);
    return ret;
}
static float3 float4x4getXAxis(const float4x4* m)
{
    return *(const float3*)&m->r0;
}
static float3 float4x4getYAxis(const float4x4* m)
{
    return *(const float3*)&m->r1;
}
static float3 float4x4getZAxis(const float4x4* m)
{
    return *(const float3*)&m->r2;
}
static float3 float4x4getPosition(const float4x4* m)
{
    return *(const float3*)&m->r3;
}
static float4x4 float4x4orthogonal(const float4x4* m)
{
    float4x4 res;
    float3 x = float4x4getXAxis(m);
    float3 y = float4x4getYAxis(m);
    float3 z;
    float3 cross;

    x = float3normalize(&x);
    cross = float3cross(&x, &y);
    z = float3normalize(&cross);
    cross = float3cross(&z, &x);
    y = float3normalize(&cross);

    {
        float4x4 t =
        {
            {    x.x,     x.y,     x.z, 0.0f},
            {    y.x,     y.y,     y.z, 0.0f},
            {    z.x,     z.y,     z.z, 0.0f},
            {m->r3.x, m->r3.y, m->r3.z, 1.0f}
        };
        res = t;
    }

    return res;
}

/* Matrix construction */
static float4x4 float4x4RotationX(float rad)
{
    float       c = cosf( rad );
    float       s = sinf( rad );
    float4x4    m = { { 1.0f, 0.0f, 0.0f, 0.0f},
                      { 0.0f,    c,    s, 0.0f},
                      { 0.0f,   -s,    c, 0.0f}, 
                      { 0.0f, 0.0f, 0.0f, 1.0f} };
    return m;
}
static float4x4 float4x4RotationY(float rad)
{
    float       c = cosf( rad );
    float       s = sinf( rad );
    float4x4    m = { {    c, 0.0f,   -s, 0.0f},
                      { 0.0f, 1.0f, 0.0f, 0.0f},
                      {    s, 0.0f,    c, 0.0f},
                      { 0.0f, 0.0f, 0.0f, 1.0f} };
    return m;
}
static float4x4 float4x4RotationZ(float rad)
{

    float       c = cosf( rad );
    float       s = sinf( rad );
    float4x4    m = { {    c,    s, 0.0f, 0.0f},
                      {   -s,    c, 0.0f, 0.0f},
                      { 0.0f, 0.0f, 1.0f, 0.0f},
                      { 0.0f, 0.0f, 0.0f, 1.0f} };
    return m;
}

static float4x4 float4x4RotationAxis(const float3* axis, float rad )
{
    float3 normAxis = float3normalize(axis);
    float c = cosf(rad);
    float s = sinf(rad);
    float t = 1 - c;

    float x = normAxis.x;
    float y = normAxis.y;
    float z = normAxis.z;

    float4x4 m = float4x4identity();

    m.r0.x = (t * x * x) + c;
    m.r0.y = (t * x * y) + s * z;
    m.r0.z = (t * x * z) - s * y;

    m.r1.x = (t * x * y) - (s * z);
    m.r1.y = (t * y * y) + c;
    m.r1.z = (t * y * z) + (s * x);

    m.r2.x = (t * x * z) + (s * y);
    m.r2.y = (t * y * z) - (s * x);
    m.r2.z = (t * z * z) + c;

    return m;
}

static float4x4 float4x4Scale(float x, float y, float z)
{
    float4x4 m = {{x,0,0,0},
                  {0,y,0,0},
                  {0,0,z,0},
                  {0,0,0,1}};
    return m;
}
static float4x4 float4x4translation(float x, float y, float z)
{
    float4x4 m = 
    {
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {   x,    y,    z, 1.0f}
    };
    return m;
}
static float4x4 float4x4translationVector(const float3* t)
{
    float4x4 m = 
    {
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {t->x, t->y, t->z, 1.0f}
    };
    return m;
}

static float4x4 float4x4PerspectiveFovLH(float fov, float aspect, float nearPlane, float farPlane)
{
    float4x4 m = float4x4identity();

    float f = tanf(kPiDiv2 - (fov/2));
    float diff = farPlane-nearPlane;
    float div = farPlane / diff;

    m.r0.x = f/aspect;
    m.r1.y = f;
    m.r2.z = div;
    m.r2.w = 1;
    m.r3.z = -nearPlane*div;
    m.r3.w = 0;

    return m;
}
static float4x4 float4x4PerspectiveFovRH(float fov, float aspect, float nearPlane, float farPlane)
{
    float4x4 m = float4x4identity();

    float f = tanf(kPiDiv2 - (fov/2));
    float diff = nearPlane-farPlane;
    float div = farPlane / diff;

    m.r0.x = f / aspect;
    m.r1.y = f;
    m.r2.z = div;
    m.r2.w = -1;
    m.r3.z = nearPlane * div;
    m.r3.w = 0;
    return m;
}
static float4x4 float4x4PerspectiveLH(float width, float height, float nearPlane, float farPlane)
{
    float4x4 m = float4x4identity();

    m.r0.x = 2*nearPlane/width;
    m.r1.y = 2*nearPlane/height;
    m.r2.z = farPlane/(farPlane-nearPlane);
    m.r2.w = 1;
    m.r3.z = nearPlane*farPlane/(nearPlane-farPlane);
    m.r3.w = 0;
    return m;
}
static float4x4 float4x4PerspectiveRH(float width, float height, float nearPlane, float farPlane)
{
    float4x4 m = float4x4identity();

    m.r0.x = 2*nearPlane/width;
    m.r1.y = 2*nearPlane/height;
    m.r2.z = farPlane/(nearPlane-farPlane);
    m.r2.w = -1;
    m.r3.z = nearPlane*farPlane/(nearPlane-farPlane);
    m.r3.w = 0;
    return m;
}
static float4x4 float4x4OrthographicOffCenterLH(float left, float right, float top, float bottom, float nearPlane, float farPlane)
{
    float4x4 m = float4x4identity();

    float diff = farPlane-nearPlane;

    m.r0.x = 2.0f/(right-left);
    m.r1.y = 2.0f/(top-bottom);
    m.r2.z = 1.0f/diff;
    m.r3.x = -((left+right)/(right-left));
    m.r3.y = -((top+bottom)/(top-bottom));
    m.r3.z = -nearPlane/diff;

    return m;
}
static float4x4 float4x4OrthographicOffCenterRH(float left, float right, float top, float bottom, float nearPlane, float farPlane)
{
    float4x4 m = float4x4identity();
    float diff = nearPlane-farPlane;

    m.r0.x = 2.0f/(right-left);
    m.r1.y = 2.0f/(top-bottom);
    m.r2.z = 1.0f/diff;
    m.r3.x = -((left+right)/(right-left));
    m.r3.y = -((top+bottom)/(top-bottom));
    m.r3.z = nearPlane/diff;

    return m;
}
static float4x4 float4x4OrthographicLH(float width, float height, float nearPlane, float farPlane)
{
    float halfWidth = width/2.0f;
    float halfHeight = height/2.0f;

    return float4x4OrthographicOffCenterLH(-halfWidth, halfWidth, halfHeight, -halfHeight, nearPlane, farPlane);
}
static float4x4 float4x4OrthographicRH(float width, float height, float nearPlane, float farPlane)
{
    float halfWidth = width/2.0f;
    float halfHeight = height/2.0f;

    return float4x4OrthographicOffCenterRH(-halfWidth, halfWidth, halfHeight, -halfHeight, nearPlane, farPlane);
}

/*
 * Quaternion math
 */
static quaternion quaternionIdentity(void)
{
    quaternion q = {0,0,0,1};
    return q;
}
static quaternion quaternionFromAxisAngle(const float3* axis, float angle)
{
    quaternion q;
    float3 norm = float3normalize(axis);
    float   a = angle*0.5f;
    float   s = sinf(a);
    q.x = norm.x*s;
    q.y = norm.y*s;
    q.z = norm.z*s;
    q.w = cosf(a);

    return q;
}
static quaternion quaternionNormalize(const quaternion* q)
{
    return float4normalize(q);
}
static float3 quaternionGetXAxis(const quaternion* q)
{
    float3 ret = { 1-2*(q->y*q->y+q->z*q->z),
                     2*(q->x*q->y+q->w*q->z),
                     2*(q->x*q->z-q->y*q->w) };
    return ret;
}
static float3 quaternionGetYAxis(const quaternion* q)
{
    float3 ret = {   2*(q->x*q->y-q->z*q->w),
                   1-2*(q->x*q->x+q->z*q->z),
                     2*(q->y*q->z+q->x*q->w) };
    return ret;
}
static float3 quaternionGetZAxis(const quaternion* q)
{
    float3 ret = {  2*(q->x*q->z+q->y*q->w),
                    2*(q->y*q->z-q->x*q->w),
                  1-2*(q->x*q->x+q->y*q->y) };
    return ret;
}
static float3x3 quaternionGetMatrix(const quaternion* q)
{
    quaternion norm = quaternionNormalize(q);
    float xx = norm.x * norm.x;
    float yy = norm.y * norm.y;
    float zz = norm.z * norm.z;

    float xy = norm.x * norm.y;
    float zw = norm.z * norm.w;
    float xz = norm.x * norm.z;
    float yw = norm.y * norm.w;
    float yz = norm.y * norm.z;
    float xw = norm.x * norm.w;

    float3x3 ret = {1 - 2*(yy+zz),     2*(xy+zw),     2*(xz-yw),
                        2*(xy-zw), 1 - 2*(xx+zz),     2*(yz+xw),
                        2*(xz+yw),     2*(yz-xw), 1 - 2*(xx+yy) };
    return ret;
}

static quaternion quaternionConjugate(const quaternion* q)
{
    quaternion ret =
    {
        -q->x,
        -q->y,
        -q->z,
         q->w
    };
    return ret;
}
static quaternion quaternionInverse(const quaternion* q)
{
    quaternion norm = quaternionNormalize(q); /* Only normalized supported now */
    return quaternionConjugate(&norm);
}
static quaternion quaternionMultiply(const quaternion* l, const quaternion* r)
{
    quaternion q = {r->w*l->x + r->x*l->w + r->y*l->z - r->z*l->y,
                    r->w*l->y + r->y*l->w + r->z*l->x - r->x*l->z,
                    r->w*l->z + r->z*l->w + r->x*l->y - r->y*l->x,
                    r->w*l->w - r->x*l->x - r->y*l->y - r->z*l->z };
    return q;
}

#endif /* Include guard */
