#include <math.h>

#include "math_linear.h"

/* Almost all of this is copied from datenwolf/linmath.h on GitHub. */

float
vec2_mag(const vec2 v)
{
	return sqrt(v[0] * v[0] + v[1] * v[1]);
}

void
vec2_div(vec2 r, const float n)
{
	r[0] /= n;
	r[1] /= n;
}

void
vec2_norm(vec2 r, const vec2 v)
{
	float m = vec2_mag(v);
	r[0] = v[0];
	r[1] = v[1];
	vec2_div(r, m);
}

void
vec3_sub(vec3 r, vec3 const a, vec3 const b)
{
    int i;
    for (i = 0; i < 3; i++)
        r[i] = a[i] - b[i];
}

void
vec3_scale(vec3 r, vec3 const v, float const s)
{
    int i;
    for (i = 0; i < 3; i++)
        r[i] = v[i] * s;
}

float
vec3_mul_inner(vec3 const a, vec3 const b)
{
    float f = 0.0f;
    int i;
    for (i = 0; i < 3; i++)
        f += b[i] * a[i];

    return f;
}

float
vec3_len(vec3 const v)
{
    return (float)sqrt(vec3_mul_inner(v, v));
}

void
vec3_norm(vec3 r, vec3 const v)
{
    float k = 1.0f / vec3_len(v);
    vec3_scale(r, v, k);
}

void
vec4_scale(vec3 r, vec3 const v, float const s)
{
    int i;
    for (i = 0; i < 4; i++)
        r[i] = v[i] * s;
}

void
vec4_add(vec4 r, vec4 const a, vec4 const b)
{
    int i;
    for (i = 0; i < 4; i++)
        r[i] = a[i] + b[i];
}

void
vec4_sub(vec4 r, vec4 const a, vec4 const b)
{
    int i;
    for (i = 0; i < 4; i++)
        r[i] = a[i] - b[i];
}

void
mat4x4_identity(mat4x4 M)
{
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            M[i][j] = i == j ? 1.0f : 0.0f;
}

void
mat4x4_translate(mat4x4 T, float x, float y, float z)
{
    mat4x4_identity(T);
    T[3][0] = x;
    T[3][1] = y;
    T[3][2] = z;
}

void
mat4x4_dup(mat4x4 R, mat4x4 M)
{
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            R[i][j] = M[i][j];
}

void
mat4x4_orthonormalize(mat4x4 R, mat4x4 M)
{
    float s = 1.0;
    vec3 h;

    mat4x4_dup(R, M);
    vec3_norm(R[2], R[2]);

    s = vec3_mul_inner(R[1], R[2]);
    vec3_scale(h, R[2], s);
    vec3_sub(R[1], R[1], h);
    vec3_norm(R[2], R[2]);

    s = vec3_mul_inner(R[1], R[2]);
    vec3_scale(h, R[2], s);
    vec3_sub(R[1], R[1], h);
    vec3_norm(R[1], R[1]);

    s = vec3_mul_inner(R[0], R[1]);
    vec3_scale(h, R[1], s);
    vec3_sub(R[0], R[0], h);
    vec3_norm(R[0], R[0]);
}

void
mat4x4_ortho(
    mat4x4 M, float l, float r, float b, float t, float n, float f)
{
    M[0][0] = 2.f / (r - l);
    M[0][1] = M[0][2] = M[0][3] = 0.f;

    M[1][1] = 2.f / (t - b);
    M[1][0] = M[1][2] = M[1][3] = 0.f;

    M[2][2] = -2.f / (f - n);
    M[2][0] = M[2][1] = M[2][3] = 0.f;

    M[3][0] = -(r + l) / (r - l);
    M[3][1] = -(t + b) / (t - b);
    M[3][2] = -(f + n) / (f - n);
    M[3][3] = 1.f;
}

void
mat4x4_scale_aniso(mat4x4 M, mat4x4 a, float x, float y, float z)
{
    int i;
    vec4_scale(M[0], a[0], x);
    vec4_scale(M[1], a[1], y);
    vec4_scale(M[2], a[2], z);
    for (i = 0; i < 4; i++)
        M[3][i] = a[3][i];
}

void
mat4x4_from_vec3_mul_outer(mat4x4 M, vec3 a, vec3 b)
{
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            M[i][j] = i < 3 && j < 3 ? a[i] * b[j] : 0.0f;
}

void
mat4x4_scale(mat4x4 M, mat4x4 a, float k)
{
    int i;
    for (i = 0; i < 4; i++)
        vec4_scale(M[i], a[i], k);
}

void
mat4x4_sub(mat4x4 M, mat4x4 a, mat4x4 b)
{
    int i;
    for (i = 0; i < 4; i++)
        vec4_sub(M[i], a[i], b[i]);
}

void
mat4x4_add(mat4x4 M, mat4x4 a, mat4x4 b)
{
    int i;
    for (i = 0; i < 4; i++)
        vec4_add(M[i], a[i], b[i]);
}

void
mat4x4_mul(mat4x4 M, mat4x4 a, mat4x4 b)
{
    mat4x4 temp;
    int k, r, c;
    for (c = 0; c < 4; c++) {
        for (r = 0; r < 4; r++) {
            temp[c][r] = 0.0f;
            for (k = 0; k < 4; k++)
                temp[c][r] += a[k][r] * b[c][k];
        }
    }
    mat4x4_dup(M, temp);
}

void
mat4x4_rotate(
    mat4x4 R, mat4x4 M, float x, float y, float z, float angle)
{
    float s = sinf(angle);
    float c = cosf(angle);
    vec3 u;
    u[0] = x;
    u[1] = y;
    u[2] = z;

    if (vec3_len(u) > 1e-4) {
        mat4x4 T, C, S = {{0}};

        vec3_norm(u, u);
        mat4x4_from_vec3_mul_outer(T, u, u);

        S[1][2] = u[0];
        S[2][1] = -u[0];
        S[2][0] = u[1];
        S[0][2] = -u[1];
        S[0][1] = u[2];
        S[1][0] = -u[2];

        mat4x4_scale(S, S, s);

        mat4x4_identity(C);
        mat4x4_sub(C, C, T);

        mat4x4_scale(C, C, c);

        mat4x4_add(T, T, C);
        mat4x4_add(T, T, S);

        T[3][3] = 1.;
        mat4x4_mul(R, M, T);
    } else {
        mat4x4_dup(R, M);
    }
}

void
mat4x4_invert(vec4 *T, vec4 *M)
{
	float idet;
	float s[6];
	float c[6];
	s[0] = M[0][0]*M[1][1] - M[1][0]*M[0][1];
	s[1] = M[0][0]*M[1][2] - M[1][0]*M[0][2];
	s[2] = M[0][0]*M[1][3] - M[1][0]*M[0][3];
	s[3] = M[0][1]*M[1][2] - M[1][1]*M[0][2];
	s[4] = M[0][1]*M[1][3] - M[1][1]*M[0][3];
	s[5] = M[0][2]*M[1][3] - M[1][2]*M[0][3];

	c[0] = M[2][0]*M[3][1] - M[3][0]*M[2][1];
	c[1] = M[2][0]*M[3][2] - M[3][0]*M[2][2];
	c[2] = M[2][0]*M[3][3] - M[3][0]*M[2][3];
	c[3] = M[2][1]*M[3][2] - M[3][1]*M[2][2];
	c[4] = M[2][1]*M[3][3] - M[3][1]*M[2][3];
	c[5] = M[2][2]*M[3][3] - M[3][2]*M[2][3];

	/* Assumes it is invertible */
	idet = 1.0f/( s[0]*c[5]-s[1]*c[4]+s[2]*c[3]+s[3]*c[2]-s[4]*c[1]+s[5]*c[0] );

	T[0][0] = ( M[1][1] * c[5] - M[1][2] * c[4] + M[1][3] * c[3]) * idet;
	T[0][1] = (-M[0][1] * c[5] + M[0][2] * c[4] - M[0][3] * c[3]) * idet;
	T[0][2] = ( M[3][1] * s[5] - M[3][2] * s[4] + M[3][3] * s[3]) * idet;
	T[0][3] = (-M[2][1] * s[5] + M[2][2] * s[4] - M[2][3] * s[3]) * idet;

	T[1][0] = (-M[1][0] * c[5] + M[1][2] * c[2] - M[1][3] * c[1]) * idet;
	T[1][1] = ( M[0][0] * c[5] - M[0][2] * c[2] + M[0][3] * c[1]) * idet;
	T[1][2] = (-M[3][0] * s[5] + M[3][2] * s[2] - M[3][3] * s[1]) * idet;
	T[1][3] = ( M[2][0] * s[5] - M[2][2] * s[2] + M[2][3] * s[1]) * idet;

	T[2][0] = ( M[1][0] * c[4] - M[1][1] * c[2] + M[1][3] * c[0]) * idet;
	T[2][1] = (-M[0][0] * c[4] + M[0][1] * c[2] - M[0][3] * c[0]) * idet;
	T[2][2] = ( M[3][0] * s[4] - M[3][1] * s[2] + M[3][3] * s[0]) * idet;
	T[2][3] = (-M[2][0] * s[4] + M[2][1] * s[2] - M[2][3] * s[0]) * idet;

	T[3][0] = (-M[1][0] * c[3] + M[1][1] * c[1] - M[1][2] * c[0]) * idet;
	T[3][1] = ( M[0][0] * c[3] - M[0][1] * c[1] + M[0][2] * c[0]) * idet;
	T[3][2] = (-M[3][0] * s[3] + M[3][1] * s[1] - M[3][2] * s[0]) * idet;
	T[3][3] = ( M[2][0] * s[3] - M[2][1] * s[1] + M[2][2] * s[0]) * idet;
}
