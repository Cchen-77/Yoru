#pragma once
#include <cassert>
#include <cmath>

class Vector2 {
public:
    Vector2() : x(0), y(0) {}
    Vector2(double x, double y) : x(x), y(y) {}

    Vector2 operator+(const Vector2 &v) const {
        return Vector2(x + v.x, y + v.y);
    }

    Vector2 operator-(const Vector2 &v) const {
        return Vector2(x - v.x, y - v.y);
    }

    Vector2 operator*(double scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    Vector2 operator/(double scalar) const {
        assert(scalar != 0);
        return Vector2(x / scalar, y / scalar);
    }

    double Length() const {
        return std::sqrt(x * x + y * y);
    }

    Vector2 Normalize() const {
        double len = Length();
        assert(len != 0);
        return *this / len;
    }

    double Dot(const Vector2 &v) const {
        return x * v.x + y * v.y;
    }

public:
    double x, y;
};
inline double Dot(const Vector2 &lhs, const Vector2 &rhs) {
    return lhs.Dot(rhs);
}
inline Vector2 Normalized(const Vector2 &v) {
    return v.Normalize();
}
inline Vector2 Min(const Vector2 &lhs, const Vector2 &rhs) {
    return {std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y)};
}
inline Vector2 Max(const Vector2 &lhs, const Vector2 &rhs) {
    return {std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y)};
}

class Point2 {
public:
    Point2() : x(0), y(0) {}
    Point2(double x, double y) : x(x), y(y) {}
    explicit Point2(const Vector2 &v) : x(v.x), y(v.y){};

    Vector2 operator-(const Point2 &p) const {
        return Vector2(x - p.x, y - p.y);
    }

    Point2 operator+(const Vector2 &v) const {
        return Point2(x + v.x, y + v.y);
    }

    Point2 operator-(const Vector2 &v) const {
        return Point2(x - v.x, y - v.y);
    }

    Point2 operator*(double scalar) const {
        return Point2(scalar * x, scalar * y);
    }

    Point2 operator/(double scalar) const {
        assert(scalar != 0);
        return Point2(x / scalar, y / scalar);
    }

public:
    double x, y;
};
inline Point2 Min(const Point2 &lhs, const Point2 &rhs) {
    return {std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y)};
}
inline Point2 Max(const Point2 &lhs, const Point2 &rhs) {
    return {std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y)};
}

class Vector3 {
public:
    Vector3() : x(0), y(0), z(0) {}
    Vector3(double x, double y, double z) : x(x), y(y), z(z) {}

    Vector3 operator+(const Vector3 &v) const {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }

    Vector3 operator-(const Vector3 &v) const {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }

    Vector3 operator*(double scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }
    Vector3 &operator*=(double scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    Vector3 operator/(double scalar) const {
        assert(scalar != 0);
        return Vector3(x / scalar, y / scalar, z / scalar);
    }
    Vector3 &operator/=(double scalar) {
        assert(scalar != 0);
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    double Length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vector3 Normalize() const {
        double len = Length();
        assert(len != 0);
        return *this / len;
    }

    double Dot(const Vector3 &v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    Vector3 Cross(const Vector3 &v) const {
        return Vector3(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x);
    }

public:
    double x, y, z;
};

inline double Dot(const Vector3 &lhs, const Vector3 &rhs) {
    return lhs.Dot(rhs);
}
inline Vector3 Cross(const Vector3 &lhs, const Vector3 &rhs) {
    return lhs.Cross(rhs);
}
inline Vector3 Normalized(const Vector3 &v) {
    return v.Normalize();
}
inline Vector3 Min(const Vector3 &lhs, const Vector3 &rhs) {
    return {std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z)};
}
inline Vector3 Max(const Vector3 &lhs, const Vector3 &rhs) {
    return {std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z)};
}

class Point3 {
public:
    Point3() : x(0), y(0), z(0) {}
    Point3(double x, double y, double z) : x(x), y(y), z(z) {}
    explicit Point3(const Vector3 &v) : x(v.x), y(v.y), z(v.z){};

    Vector3 operator-(const Point3 &p) const {
        return Vector3(x - p.x, y - p.y, z - p.z);
    }

    Point3 operator+(const Vector3 &v) const {
        return Point3(x + v.x, y + v.y, z + v.z);
    }

    Point3 operator-(const Vector3 &v) const {
        return Point3(x - v.x, y - v.y, z - v.z);
    }

    Point3 operator*(double scalar) const {
        return Point3(scalar * x, scalar * y, scalar * z);
    }
    Point3 operator/(double scalar) const {
        assert(scalar != 0);
        return Point3(x / scalar, y / scalar, z / scalar);
    }

public:
    double x, y, z;
};
inline Point3 Min(const Point3 &lhs, const Point3 &rhs) {
    return {std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z)};
}
inline Point3 Max(const Point3 &lhs, const Point3 &rhs) {
    return {std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z)};
}