#ifndef MATH_LINEAR_INCLUDED
#define MATH_LINEAR_INCLUDED
#include "inttypes.h"

#ifndef sinf
#define sinf(x) (float)sin((double)(x))
#endif
#ifndef cosf
#define cosf(x) (float)cos((double)(x))
#endif

typedef float vec2[2];
typedef float vec3[3];
typedef float vec4[4];
typedef float mat4x4[4][4];
typedef float mat4[4][4];

typedef int32_t ivec2[2];
typedef int32_t ivec3[3];

float vec2_mag(const vec2 v);
void vec2_div(vec2 r, const float n);
void vec2_norm(vec2 r, const vec2 v);

void vec3_sub(vec3 r, vec3 const a, vec3 const b);
void vec3_scale(vec3 r, vec3 const v, float const s);
float vec3_mul_inner(vec3 const a, vec3 const b);
float vec3_len(vec3 const v);
void vec3_norm(vec3 r, vec3 const v);
void vec4_scale(vec3 r, vec3 const v, float const s);
void vec4_add(vec4 r, vec4 const a, vec4 const b);
void vec4_sub(vec4 r, vec4 const a, vec4 const b);
void mat4x4_identity(mat4x4 M);
void mat4x4_translate(mat4x4 T, float x, float y, float z);
void mat4x4_dup(mat4x4 R, mat4x4 M);
void mat4x4_orthonormalize(mat4x4 R, mat4x4 M);
void mat4x4_ortho(mat4x4 M, float l, float r, float b, float t, float n, float f);
void mat4x4_scale_aniso(mat4x4 M, mat4x4 a, float x, float y, float z);
void mat4x4_from_vec3_mul_outer(mat4x4 M, vec3 a, vec3 b);
void mat4x4_scale(mat4x4 M, mat4x4 a, float k);
void mat4x4_sub(mat4x4 M, mat4x4 a, mat4x4 b);
void mat4x4_add(mat4x4 M, mat4x4 a, mat4x4 b);
void mat4x4_mul(mat4x4 M, mat4x4 a, mat4x4 b);
void mat4x4_rotate(mat4x4 R, mat4x4 M, float x, float y, float z, float angle);
void mat4x4_invert(vec4 *T, vec4 *M);

#endif
