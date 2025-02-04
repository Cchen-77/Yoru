#pragma once
#include "Vector.h"
#include <limits>
#include <cassert>
class BBox2 {
};
class BBox3 {
public:
    BBox3() {
        constexpr double maxNum = std::numeric_limits<double>::max();
        constexpr double minNum = std::numeric_limits<double>::min();
        pMin = {maxNum, maxNum, maxNum};
        pMax = {minNum, minNum, minNum};
    }
    BBox3(const Point3 &point) : pMin(point), pMax(point){};
    BBox3(const Point3 &p0, const Point3 &p1) : pMin(Min(p0, p1)), pMax(Max(p0, p1)){};

    Point3 Lerp(const Vector3 &t) const {
        assert(!isDegenerate());
        return {std::lerp(pMin.x, pMax.x, t.x), std::lerp(pMin.y, pMax.y, t.y), std::lerp(pMin.z, pMax.z, t.z)};
    }
    Point3 Offset(const Point3 &p) const {
        assert(!isDegenerate());
        Vector3 v = p - pMin;
        Vector3 scale = pMax - pMin;
        if (scale.x > 0) v.x /= scale.x;
        if (scale.y > 0) v.y /= scale.y;
        if (scale.z > 0) v.z /= scale.z;
        return Point3(v);
    }

    bool isEmpty() const {
        return pMin.x >= pMax.x || pMin.y >= pMax.y || pMin.z >= pMax.z;
    }
    bool isDegenerate() const {
        return pMin.x > pMax.x || pMin.y > pMax.y || pMin.z > pMax.z;
    }

    void Union(const Point3 &p) {
        pMin = Min(pMin, p);
        pMax = Max(pMax, p);
    }
    void Union(const BBox3 &other) {
        if (other.isDegenerate()) return;
        pMin = Min(pMin, other.pMin);
        pMax = Max(pMax, other.pMax);
    }
    BBox3 operator+(const Point3 &p) const {
        return BBox3(Min(pMin, p), Max(pMax, p));
    }
    BBox3 operator+(const BBox3 &other) const {
        if (other.isDegenerate()) return *this;
        if (isDegenerate()) return other;
        return BBox3(Min(pMin, other.pMin), Max(pMax, other.pMax));
    }

    bool isInside(const Point3 &p) const {
        return p.x >= pMin.x && p.x <= pMax.x && p.y >= pMin.y && p.y <= pMax.y && p.z >= pMin.z && p.z <= pMax.z;
    }

private:
    Point3 pMin;
    Point3 pMax;
};