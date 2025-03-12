#pragma once
#include <cassert>
#include <cmath>
#include <algorithm>// for std::min and std::max

template<typename T>
class TPoint2;

template<typename T>
class TVector2 {
public:
    TVector2() : x(0), y(0) {}
    TVector2(T c) : x(c), y(c) {}
    TVector2(T x, T y) : x(x), y(y) {}
    explicit TVector2(const TPoint2<T> &p) : x(p.x), y(p.y){};

    TVector2 operator+(const TVector2 &v) const {
        return TVector2(x + v.x, y + v.y);
    }

    TVector2 operator-(const TVector2 &v) const {
        return TVector2(x - v.x, y - v.y);
    }

    TVector2 operator-() const {
        return TVector2(-x, -y);
    }

    TVector2 operator*(T scalar) const {
        return TVector2(x * scalar, y * scalar);
    }

    TVector2 operator/(T scalar) const {
        assert(scalar != 0);
        return TVector2(x / scalar, y / scalar);
    }

    T &operator[](int i) {
        if (i == 0) {
            return x;
        } else {
            return y;
        }
    }

    T operator[](int i) const {
        if (i == 0) {
            return x;
        } else {
            return y;
        }
    }

    T Length() const {
        return std::sqrt(x * x + y * y);
    }

    T Length2() const {
        return x * x + y * y;
    }

    TVector2 Normalize() const {
        T len = Length();
        assert(len != 0);
        return *this / len;
    }

    T Dot(const TVector2 &v) const {
        return x * v.x + y * v.y;
    }

public:
    T x, y;
};

template<typename T>
inline T Dot(const TVector2<T> &lhs, const TVector2<T> &rhs) {
    return lhs.Dot(rhs);
}

template<typename T>
inline TVector2<T> Normalized(const TVector2<T> &v) {
    return v.Normalize();
}

template<typename T>
inline TVector2<T> Min(const TVector2<T> &lhs, const TVector2<T> &rhs) {
    return {std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y)};
}

template<typename T>
inline TVector2<T> Max(const TVector2<T> &lhs, const TVector2<T> &rhs) {
    return {std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y)};
}

template<typename T>
class TPoint2 {
public:
    TPoint2() : x(0), y(0) {}
    TPoint2(T c) : x(c), y(c) {}
    TPoint2(T x, T y) : x(x), y(y) {}
    explicit TPoint2(const TVector2<T> &v) : x(v.x), y(v.y) {}

    TVector2<T> operator-(const TPoint2 &p) const {
        return TVector2<T>(x - p.x, y - p.y);
    }

    TPoint2 operator+(const TVector2<T> &v) const {
        return TPoint2(x + v.x, y + v.y);
    }

    TPoint2 operator+(const TPoint2 &p) const {
        return TPoint2(x + p.x, y + p.y);
    }

    TPoint2 operator-(const TVector2<T> &v) const {
        return TPoint2(x - v.x, y - v.y);
    }

    TPoint2 operator*(T scalar) const {
        return TPoint2(scalar * x, scalar * y);
    }

    TPoint2 operator/(T scalar) const {
        assert(scalar != 0);
        return TPoint2(x / scalar, y / scalar);
    }

    T &operator[](int i) {
        if (i == 0) {
            return x;
        } else {
            return y;
        }
    }

    T operator[](int i) const {
        if (i == 0) {
            return x;
        } else {
            return y;
        }
    }

public:
    T x, y;
};

template<typename T>
inline TPoint2<T> Min(const TPoint2<T> &lhs, const TPoint2<T> &rhs) {
    return {std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y)};
}

template<typename T>
inline TPoint2<T> Max(const TPoint2<T> &lhs, const TPoint2<T> &rhs) {
    return {std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y)};
}

template<typename T>
class TPoint3;

template<typename T>
class TVector3 {
public:
    TVector3() : x(0), y(0), z(0) {}
    TVector3(T c) : x(c), y(c), z(c) {}
    TVector3(T x, T y, T z) : x(x), y(y), z(z) {}
    explicit TVector3(const TPoint3<T> &p) : x(p.x), y(p.y), z(p.z){};

    TVector3 operator+(const TVector3 &v) const {
        return TVector3(x + v.x, y + v.y, z + v.z);
    }

    TVector3 operator-(const TVector3 &v) const {
        return TVector3(x - v.x, y - v.y, z - v.z);
    }

    TVector3 operator-() const {
        return TVector3(-x, -y, -z);
    }

    TVector3 operator*(T scalar) const {
        return TVector3(x * scalar, y * scalar, z * scalar);
    }

    TVector3 &operator*=(T scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    TVector3 operator/(T scalar) const {
        assert(scalar != 0);
        return TVector3(x / scalar, y / scalar, z / scalar);
    }

    TVector3 &operator/=(T scalar) {
        assert(scalar != 0);
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    T &operator[](int i) {
        if (i == 0) {
            return x;
        } else if (i == 1) {
            return y;
        } else {
            return z;
        }
    }

    T operator[](int i) const {
        if (i == 0) {
            return x;
        } else if (i == 1) {
            return y;
        } else {
            return z;
        }
    }

    T Length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    T Length2() const {
        return x * x + y * y + z * z;
    }

    TVector3 Normalize() const {
        T len = Length();
        assert(len != 0);
        return *this / len;
    }

    T Dot(const TVector3 &v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    TVector3 Cross(const TVector3 &v) const {
        return TVector3(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x);
    }

public:
    T x, y, z;
};

template<typename T>
inline T Dot(const TVector3<T> &lhs, const TVector3<T> &rhs) {
    return lhs.Dot(rhs);
}

template<typename T>
inline T AbsDot(const TVector3<T> &lhs, const TVector3<T> &rhs) {
    return std::abs(lhs.Dot(rhs));
}

template<typename T>
inline TVector3<T> Cross(const TVector3<T> &lhs, const TVector3<T> &rhs) {
    return lhs.Cross(rhs);
}

template<typename T>
inline TVector3<T> Normalized(const TVector3<T> &v) {
    return v.Normalize();
}

template<typename T>
inline TVector3<T> Min(const TVector3<T> &lhs, const TVector3<T> &rhs) {
    return {std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z)};
}

template<typename T>
inline TVector3<T> Max(const TVector3<T> &lhs, const TVector3<T> &rhs) {
    return {std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z)};
}

template<typename T>
class TPoint3 {
public:
    TPoint3() : x(0), y(0), z(0) {}
    TPoint3(T c) : x(x), y(c), z(c) {}
    TPoint3(T x, T y, T z) : x(x), y(y), z(z) {}
    explicit TPoint3(const TVector3<T> &v) : x(v.x), y(v.y), z(v.z) {}

    TVector3<T> operator-(const TPoint3 &p) const {
        return TVector3<T>(x - p.x, y - p.y, z - p.z);
    }

    TPoint3 operator+(const TVector3<T> &v) const {
        return TPoint3(x + v.x, y + v.y, z + v.z);
    }

    TPoint3 operator+(const TPoint3 &p) const {
        return TPoint3(x + p.x, y + p.y, z + p.z);
    }

    TPoint3 operator-(const TVector3<T> &v) const {
        return TPoint3(x - v.x, y - v.y, z - v.z);
    }

    TPoint3 operator*(T scalar) const {
        return TPoint3(scalar * x, scalar * y, scalar * z);
    }

    TPoint3 operator/(T scalar) const {
        assert(scalar != 0);
        return TPoint3(x / scalar, y / scalar, z / scalar);
    }

    T &operator[](int i) {
        if (i == 0) {
            return x;
        } else if (i == 1) {
            return y;
        } else {
            return z;
        }
    }

    T operator[](int i) const {
        if (i == 0) {
            return x;
        } else if (i == 1) {
            return y;
        } else {
            return z;
        }
    }

public:
    T x, y, z;
};

template<typename T>
inline TPoint3<T> Min(const TPoint3<T> &lhs, const TPoint3<T> &rhs) {
    return {std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z)};
}

template<typename T>
inline TPoint3<T> Max(const TPoint3<T> &lhs, const TPoint3<T> &rhs) {
    return {std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z)};
}

using Vector2 = TVector2<double>;
using Point2 = TPoint2<double>;
using Vector3 = TVector3<double>;
using Point3 = TPoint3<double>;

using Vector2i = TVector2<int>;
using Point2i = TPoint2<int>;
using Vector3i = TVector3<int>;
using Point3i = TPoint3<int>;