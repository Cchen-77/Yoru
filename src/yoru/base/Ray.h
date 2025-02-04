#pragma once
#include "Vector.h"

#include <limits>
class Ray {
public:
    Ray(const Point3 &o, const Vector3 &d, double tmin = 0., double tmax = std::numeric_limits<double>::max())
        : origin(o), direction(d), timeMin(tmin), timeMax(tmax) {}
    Ray(const Ray &r) = default;

    Point3 operator()(double t) const {
        return origin + direction * t;
    }

public:
    Point3 origin;
    Vector3 direction;
    double timeMin;
    double timeMax;
};