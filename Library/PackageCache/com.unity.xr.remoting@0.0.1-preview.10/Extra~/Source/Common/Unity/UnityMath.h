#pragma once

#include <cmath>
#include "UnityXRTypes.h"

static const UnityXRVector3 kUp = { 0, 1, 0 };

static const UnityXRPose kIdentityPose = UnityXRPose{UnityXRVector3{0, 0, 0}, UnityXRVector4{0, 0, 0, 1}};

static inline UnityXRMatrix4x4 Identity()
{
    return UnityXRMatrix4x4
    {
        UnityXRVector4{1, 0, 0, 0},
        UnityXRVector4{0, 1, 0, 0},
        UnityXRVector4{0, 0, 1, 0},
        UnityXRVector4{0, 0, 0, 1}
    };
}

static inline UnityXRMatrix4x4 Mul(const UnityXRMatrix4x4& a, const UnityXRMatrix4x4& b)
{
    UnityXRMatrix4x4 m;
    for (int i = 0; i < 4; ++i)
    {
        m.columns[i].x = a.columns[0].x * b.columns[i].x + a.columns[1].x * b.columns[i].y + a.columns[2].x * b.columns[i].z + a.columns[3].x * b.columns[i].w;
        m.columns[i].y = a.columns[0].y * b.columns[i].x + a.columns[1].y * b.columns[i].y + a.columns[2].y * b.columns[i].z + a.columns[3].y * b.columns[i].w;
        m.columns[i].z = a.columns[0].z * b.columns[i].x + a.columns[1].z * b.columns[i].y + a.columns[2].z * b.columns[i].z + a.columns[3].z * b.columns[i].w;
        m.columns[i].w = a.columns[0].w * b.columns[i].x + a.columns[1].w * b.columns[i].y + a.columns[2].w * b.columns[i].z + a.columns[3].w * b.columns[i].w;
    }

    return m;
}

static inline UnityXRVector2 Mul(const UnityXRVector2& v, float f)
{
    return UnityXRVector2
    {
        v.x * f,
        v.y * f
    };
}

static inline UnityXRVector3 Mul(const UnityXRVector3& v, float f)
{
    return UnityXRVector3
    {
        v.x * f,
        v.y * f,
        v.z * f
    };
}

static inline bool Mul(const UnityXRMatrix4x4& m, const UnityXRVector3& v, UnityXRVector3* output)
{
    output->x = m.columns[0].x * v.x + m.columns[1].x * v.y + m.columns[2].x * v.z + m.columns[3].x;
    output->y = m.columns[0].y * v.x + m.columns[1].y * v.y + m.columns[2].y * v.z + m.columns[3].y;
    output->z = m.columns[0].z * v.x + m.columns[1].z * v.y + m.columns[2].z * v.z + m.columns[3].z;
    float w   = m.columns[0].w * v.x + m.columns[1].w * v.y + m.columns[2].w * v.z + m.columns[3].w;
    if (std::abs(w) <= 1.0e-7f)
        return false;

    float invW = 1.0f / w;
    output->x *= invW;
    output->y *= invW;
    output->z *= invW;
    return true;
}

static inline UnityXRVector3 Add(const UnityXRVector3& a, const UnityXRVector3& b)
{
    return UnityXRVector3
    {
        a.x + b.x,
        a.y + b.y,
        a.z + b.z
    };
}

static inline UnityXRVector2 Sub(const UnityXRVector2& a, const UnityXRVector2& b)
{
    return UnityXRVector2
    {
        a.x - b.x,
        a.y - b.y
    };
}

static inline UnityXRVector3 Sub(const UnityXRVector3& a, const UnityXRVector3& b)
{
    return UnityXRVector3
    {
        a.x - b.x,
        a.y - b.y,
        a.z - b.z
    };
}

static inline float Dot(const UnityXRVector3& a, const UnityXRVector3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline float LengthSquared(const UnityXRVector3& v)
{
    return Dot(v, v);
}

static inline float Length(const UnityXRVector3& v)
{
    return std::sqrt(LengthSquared(v));
}

static inline UnityXRVector3 Normalize(const UnityXRVector3& v)
{
    return Mul(v, 1.f / Length(v));
}

static inline UnityXRVector4 Inverse(const UnityXRVector4& rotation)
{
    return UnityXRVector4
    {
        -rotation.x,
        -rotation.y,
        -rotation.z,
        rotation.w
    };
}

static inline UnityXRMatrix4x4 InverseTransform(const UnityXRMatrix4x4& m)
{
    return UnityXRMatrix4x4
    {
        UnityXRVector4{ m.columns[0].x,  m.columns[1].x,  m.columns[2].x, m.columns[0].w},
        UnityXRVector4{ m.columns[0].y,  m.columns[1].y,  m.columns[2].y, m.columns[1].w},
        UnityXRVector4{ m.columns[0].z,  m.columns[1].z,  m.columns[2].z, m.columns[2].w},
        UnityXRVector4{-m.columns[3].x, -m.columns[3].y, -m.columns[3].z, m.columns[3].w}
    };
}

static inline UnityXRVector3 Mul(const UnityXRVector3& a, const UnityXRVector3& b)
{
    return UnityXRVector3
    {
        a.x * b.x,
        a.y * b.y,
        a.z * b.z
    };
}

static inline UnityXRVector3 Mul(const UnityXRVector4& q, const UnityXRVector3& u)
{
    const UnityXRVector3 c0 = { -2, +2, -2 };
    const UnityXRVector3 c1 = { -2, -2, +2 };
    const UnityXRVector3 c2 = { +2, -2, -2 };

    const UnityXRVector3 qyxw = { q.y, q.x, q.w };
    const UnityXRVector3 qzwx = { q.z, q.w, q.x };
    const UnityXRVector3 qwzy = { q.w, q.z, q.y };

    const auto m0 = Sub(Mul(Mul(c0, q.y), qyxw), Mul(Mul(c2, q.z), qzwx));
    const auto m1 = Sub(Mul(Mul(c1, q.z), qwzy), Mul(Mul(c0, q.x), qyxw));
    const auto m2 = Sub(Mul(Mul(c2, q.x), qzwx), Mul(Mul(c1, q.y), qwzy));

    return Add(Add(u, Mul(m0, u.x)), Add(Mul(m1, u.y), Mul(m2, u.z)));
}
