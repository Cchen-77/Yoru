#pragma once
#include "Vector.h"
#include "Ray.h"

#include <limits>
#include <cassert>
struct BBox2 {
    BBox2() {
        constexpr double maxNum = std::numeric_limits<double>::max();
        constexpr double minNum = std::numeric_limits<double>::min();
        pMin = {maxNum, maxNum};
        pMax = {minNum, minNum};
    }
    BBox2(const Point2 &point) : pMin(point), pMax(point) {}
    BBox2(const Point2 &p0, const Point2 &p1) : pMin(Min(p0, p1)), pMax(Max(p0, p1)) {}

    Point2 Lerp(const Vector2 &t) const {
        assert(!isEmpty());
        return {std::lerp(pMin.x, pMax.x, t.x), std::lerp(pMin.y, pMax.y, t.y)};
    }
    Point2 Offset(const Point2 &p) const {
        assert(!isEmpty());
        Vector2 v = p - pMin;
        Vector2 scale = pMax - pMin;
        if (scale.x > 0) v.x /= scale.x;
        if (scale.y > 0) v.y /= scale.y;
        return Point2(v);
    }

    bool isEmpty() const {
        return pMin.x > pMax.x || pMin.y > pMax.y;
    }
    bool isDegenerate() const {
        return !isEmpty() && (pMin.x == pMax.x || pMin.y == pMax.y);
    }

    void Union(const Point2 &p) {
        pMin = Min(pMin, p);
        pMax = Max(pMax, p);
    }
    void Union(const BBox2 &other) {
        if (other.isEmpty()) return;
        pMin = Min(pMin, other.pMin);
        pMax = Max(pMax, other.pMax);
    }
    void Intersect(const BBox2 &other) {
        if (other.isEmpty()) {
            *this = other;
            return;
        }
        pMin = Max(pMin, other.pMin);
        pMax = Min(pMax, other.pMax);
    }
    BBox2 operator+(const Point2 &p) const {
        return BBox2(Min(pMin, p), Max(pMax, p));
    }
    BBox2 operator+(const BBox2 &other) const {
        if (other.isEmpty()) return *this;
        if (isEmpty()) return other;
        return BBox2(Min(pMin, other.pMin), Max(pMax, other.pMax));
    }
    BBox2 operator&(const BBox2 &other) const {
        if (other.isEmpty()) return other;
        if (isEmpty()) return *this;
        return BBox2(Max(pMin, other.pMin), Min(pMax, other.pMax));
    }

    bool isInside(const Point2 &p) const {
        return p.x >= pMin.x && p.x <= pMax.x && p.y >= pMin.y && p.y <= pMax.y;
    }

    Point2 pMin;
    Point2 pMax;
};

struct BBox2i {
    BBox2i() {
        constexpr int maxNum = std::numeric_limits<int>::max();
        constexpr int minNum = std::numeric_limits<int>::min();
        pMin = {maxNum, maxNum};
        pMax = {minNum, minNum};
    }

    BBox2i(const Point2i &point) : pMin(point), pMax(point) {}

    BBox2i(const Point2i &p0, const Point2i &p1) : pMin(Min(p0, p1)), pMax(Max(p0, p1)) {}

    bool isEmpty() const {
        return pMin.x >= pMax.x || pMin.y >= pMax.y;
    }

    void Union(const Point2i &p) {
        pMin = Min(pMin, p);
        pMax = Max(pMax, p);
    }

    void Union(const BBox2i &other) {
        if (other.isEmpty()) return;
        pMin = Min(pMin, other.pMin);
        pMax = Max(pMax, other.pMax);
    }

    void Intersect(const BBox2i &other) {
        if (other.isEmpty()) {
            *this = other;
            return;
        }
        pMin = Max(pMin, other.pMin);
        pMax = Min(pMax, other.pMax);
    }

    BBox2i operator+(const Point2i &p) const {
        return BBox2i(Min(pMin, p), Max(pMax, p));
    }

    BBox2i operator+(const BBox2i &other) const {
        if (other.isEmpty()) return *this;
        if (isEmpty()) return other;
        return BBox2i(Min(pMin, other.pMin), Max(pMax, other.pMax));
    }
    BBox2i operator&(const BBox2i &other) const {
        if (other.isEmpty()) return other;
        if (isEmpty()) return *this;
        return BBox2i(Max(pMin, other.pMin), Min(pMax, other.pMax));
    }

    bool isInside(const Point2i &p) const {
        return p.x >= pMin.x && p.x < pMax.x && p.y >= pMin.y && p.y < pMax.y;
    }

    Point2i pMin;
    Point2i pMax;
};

struct BBox3 {
    BBox3() {
        constexpr double maxNum = std::numeric_limits<double>::max();
        constexpr double minNum = std::numeric_limits<double>::min();
        pMin = {maxNum, maxNum, maxNum};
        pMax = {minNum, minNum, minNum};
    }
    BBox3(const Point3 &point) : pMin(point), pMax(point){};
    BBox3(const Point3 &p0, const Point3 &p1) : pMin(Min(p0, p1)), pMax(Max(p0, p1)){};

    Point3 Lerp(const Vector3 &t) const {
        assert(!isEmpty());
        return {std::lerp(pMin.x, pMax.x, t.x), std::lerp(pMin.y, pMax.y, t.y), std::lerp(pMin.z, pMax.z, t.z)};
    }
    Point3 Offset(const Point3 &p) const {
        assert(!isEmpty());
        Vector3 v = p - pMin;
        Vector3 scale = pMax - pMin;
        if (scale.x > 0) v.x /= scale.x;
        if (scale.y > 0) v.y /= scale.y;
        if (scale.z > 0) v.z /= scale.z;
        return Point3(v);
    }

    bool isEmpty() const {
        return pMin.x > pMax.x || pMin.y > pMax.y || pMin.z > pMax.z;
    }
    bool isDegenerate() const {
        return !isEmpty() && (pMin.x == pMax.x || pMin.y == pMax.y || pMin.z == pMax.z);
    }

    void Union(const Point3 &p) {
        pMin = Min(pMin, p);
        pMax = Max(pMax, p);
    }
    void Union(const BBox3 &other) {
        if (other.isEmpty()) return;
        pMin = Min(pMin, other.pMin);
        pMax = Max(pMax, other.pMax);
    }
    void Intersect(const BBox3 &other) {
        if (other.isEmpty()) {
            *this = other;
            return;
        }
        pMin = Max(pMin, other.pMin);
        pMax = Min(pMax, other.pMax);
    }
    BBox3 operator+(const Point3 &p) const {
        return BBox3(Min(pMin, p), Max(pMax, p));
    }
    BBox3 operator+(const BBox3 &other) const {
        if (other.isEmpty()) return *this;
        if (isEmpty()) return other;
        return BBox3(Min(pMin, other.pMin), Max(pMax, other.pMax));
    }
    BBox3 operator&(const BBox3 &other) const {
        if (other.isEmpty()) return other;
        if (isEmpty()) return *this;
        return BBox3(Max(pMin, other.pMin), Min(pMax, other.pMax));
    }
    Point3 operator[](int i) const {
        if (i == 0) {
            return pMin;
        }
        return pMax;
    }

    bool isInside(const Point3 &p) const {
        return p.x >= pMin.x && p.x <= pMax.x && p.y >= pMin.y && p.y <= pMax.y && p.z >= pMin.z && p.z <= pMax.z;
    }

    Point3 Corner(int corner) const {
        return Point3((*this)[(corner & 1)].x,
                      (*this)[(corner & 2) ? 1 : 0].y,
                      (*this)[(corner & 4) ? 1 : 0].z);
    }

    Point3 pMin;
    Point3 pMax;

    bool Intersect(const Ray &ray, double *hitt0, double *hitt1) const;
};