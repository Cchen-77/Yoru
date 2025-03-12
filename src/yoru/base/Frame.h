#pragma once
#include "Vector.h"
#include "utils/Util.h"
struct Frame {
    Vector3 s, t, n;
    Frame() = default;

    Frame(Vector3 s, Vector3 t, Vector3 n) : s(Normalized(s)), t(Normalized(t)), n(Normalized(n)){};

    Frame(Vector3 n) : n(Normalized(n)) {
        coordinateSystem(n, s, t);
    }

    Vector3 ToLocal(Vector3 v) const {
        return {Dot(v, s), Dot(v, t), Dot(n, t)};
    }
    Vector3 FromLocal(Vector3 v) const {
        return s * v[0] + t * v[1] + n * v[2];
    }
};