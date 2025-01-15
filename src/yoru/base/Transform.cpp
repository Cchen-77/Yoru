#include "Transform.h"

#include <cstring>
Transform::Transform(const Eigen::Matrix4d &mat) {
    m_transform = mat;
    m_inverse = m_transform.inverse();
}

Transform::Transform(double mat[4][4]) {
    std::memcpy(m_transform.data(), mat, 16 * sizeof(double));
    m_inverse = m_transform.inverse();
}

Point3 Transform::operator*(const Point3 &point) const {
    Eigen::Vector4d p = {point.x, point.y, point.z, 1.};
    p = m_transform * p;
    return {p(0), p(1), p(2)};
}

Vector3 Transform::operator*(const Vector3 &vec) const {
    Eigen::Vector4d v = {vec.x, vec.y, vec.z, 0.};
    v = m_transform * v;
    return {v(0), v(1), v(2)};
}
