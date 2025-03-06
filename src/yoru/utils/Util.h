#pragma once
#include"base/Vector.h"
inline void coordinateSystem(Vector3 &a, Vector3 &b, Vector3 &c) {
    a = Normalized(a);
    if (std::abs(a.x) > std::abs(a.y)) {
        double invLen = 1.0f / std::sqrt(a.x * a.x + a.z * a.z);
        c = Vector3(a.z * invLen, 0.0f, -a.x * invLen);
    } else {
        double invLen = 1.0f / std::sqrt(a.y * a.y + a.z * a.z);
        c = Vector3(0.0f, a.z * invLen, -a.y * invLen);
    }
    b = Cross(c, a);
}

inline int PermutationElement(uint32_t i, uint32_t l, uint32_t p) {
    uint32_t w = l - 1;
    w |= w >> 1;
    w |= w >> 2;
    w |= w >> 4;
    w |= w >> 8;
    w |= w >> 16;
    do {
        i ^= p;
        i *= 0xe170893d;
        i ^= p >> 16;
        i ^= (i & w) >> 4;
        i ^= p >> 8;
        i *= 0x0929eb3f;
        i ^= p >> 23;
        i ^= (i & w) >> 1;
        i *= 1 | p >> 27;
        i *= 0x6935fa69;
        i ^= (i & w) >> 11;
        i *= 0x74dcb303;
        i ^= (i & w) >> 2;
        i *= 0x9e501cc3;
        i ^= (i & w) >> 2;
        i *= 0xc860a3df;
        i &= w;
        i ^= i >> 5;
    } while (i >= l);
    return (i + p) % l;
}