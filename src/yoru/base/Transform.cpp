#include "Transform.h"

#include <cstring>
#include <limits>
Transform::Transform(const Eigen::Matrix4d &mat) {
    m_transform = mat;
    m_inverse = m_transform.inverse();
}

Transform::Transform(double mat[4][4]) {
    std::memcpy(m_transform.data(), mat, 16 * sizeof(double));
    m_inverse = m_transform.inverse();
}

Transform Transform::operator*(const Transform &other) const {
    return Transform(m_transform * other.m_transform, other.m_inverse * m_inverse);
}

Point3 Transform::operator()(const Point3 &point) const {
    Eigen::Vector4d p = {point.x, point.y, point.z, 1.};
    p = m_transform * p;
    return {p(0), p(1), p(2)};
}

Vector3 Transform::operator()(const Vector3 &vec) const {
    Eigen::Vector4d v = {vec.x, vec.y, vec.z, 0.};
    v = m_transform * v;
    return {v(0), v(1), v(2)};
}

Ray Transform::operator()(const Ray &ray) const {
    Point3 o = ray.origin;
    o = (*this)(o);
    Vector3 d = ray.direction;
    d = (*this)(d);
    return Ray(o, d, ray.timeMin, ray.timeMax);
}

Transform Transform::Translation(double tx, double ty, double tz) {
    Eigen::Matrix4d trans;
    trans << 1, 0, 0, tx,
        0, 1, 0, ty,
        0, 0, 1, tz,
        0, 0, 0, 1;
    Eigen::Matrix4d inv;
    inv << 1, 0, 0, -tx,
        0, 1, 0, -ty,
        0, 0, 1, -tz,
        0, 0, 0, 1;
    return Transform(trans, inv);
}

Transform Transform::Scale(double scale) {
    Eigen::Matrix4d trans;
    trans << scale, 0, 0, 0,
        0, scale, 0, 0,
        0, 0, scale, 0,
        0, 0, 0, 1;
    double invScale = 1. / scale;
    Eigen::Matrix4d inv;
    inv << invScale, 0, 0, 0,
        0, invScale, 0, 0,
        0, 0, invScale, 0,
        0, 0, 0, 1;
    return Transform(trans, inv);
}

Transform Transform::Scale(double sx, double sy, double sz) {
    Eigen::Matrix4d trans;
    trans << sx, 0, 0, 0,
        0, sy, 0, 0,
        0, 0, sz, 0,
        0, 0, 0, 1;
    Eigen::Matrix4d inv;
    inv << 1. / sx, 0, 0, 0,
        0, 1. / sy, 0, 0,
        0, 0, 1. / sz, 0,
        0, 0, 0, 1;
    return Transform(trans, inv);
}

Transform Transform::RotateX(double theta) {
    double cosTheta = std::cos(theta);
    double sinTheta = std::sin(theta);
    Eigen::Matrix4d trans;
    trans << 1, 0, 0, 0,
        0, cosTheta, -sinTheta, 0,
        0, sinTheta, cosTheta, 0,
        0, 0, 0, 1;
    return Transform(trans, trans.transpose());
}

Transform Transform::RotateY(double theta) {
    double cosTheta = std::cos(theta);
    double sinTheta = std::sin(theta);
    Eigen::Matrix4d trans;
    trans << cosTheta, 0, sinTheta, 0,
        0, 1, 0, 0,
        -sinTheta, 0, cosTheta, 0,
        0, 0, 0, 1;
    return Transform(trans, trans.transpose());
}

Transform Transform::RotateZ(double theta) {
    double cosTheta = std::cos(theta);
    double sinTheta = std::sin(theta);
    Eigen::Matrix4d trans;
    trans << cosTheta, -sinTheta, 0, 0,
        sinTheta, cosTheta, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1;
    return Transform(trans, trans.transpose());
}

Transform Transform::Rotate(double cosTheta, double sinTheta, Vector3 axis) {
    Eigen::Matrix4d trans;
    trans(0, 0) = axis.x * axis.x + (1 - axis.x * axis.x) * cosTheta;
    trans(0, 1) = axis.x * axis.y * (1 - cosTheta) - axis.z * sinTheta;
    trans(0, 2) = axis.x * axis.z * (1 - cosTheta) + axis.y * sinTheta;
    trans(0, 3) = 0;

    trans(1, 0) = axis.x * axis.y * (1 - cosTheta) + axis.z * sinTheta;
    trans(1, 1) = axis.y * axis.y + (1 - axis.y * axis.y) * cosTheta;
    trans(1, 2) = axis.y * axis.z * (1 - cosTheta) - axis.x * sinTheta;
    trans(1, 3) = 0;

    trans(2, 0) = axis.x * axis.z * (1 - cosTheta) - axis.y * sinTheta;
    trans(2, 1) = axis.y * axis.z * (1 - cosTheta) + axis.x * sinTheta;
    trans(2, 2) = axis.z * axis.z + (1 - axis.z * axis.z) * cosTheta;
    trans(2, 3) = 0;

    trans(3, 3) = 1;

    return Transform(trans, trans.transpose());
}

Transform Transform::Rotate(double theta, Vector3 axis) {
    return Transform::Rotate(std::cos(theta), std::sin(theta), axis);
}

Transform Transform::LookAt(Point3 eye, Point3 center, Vector3 up) {
    Vector3 direction = Normalized(center - eye);
    Vector3 right = Normalized(Cross(up, direction));
    Vector3 newUp = Cross(direction, up);

    Eigen::Matrix4d worldFromCamera;

    worldFromCamera << -right.x, up.x, -direction.x, eye.x,
        -right.y, up.y, -direction.y, eye.y,
        -right.z, up.z, -direction.z, eye.z,
        0, 0, 0, 1;

    Eigen::Matrix4d cameraFromWorld = worldFromCamera.inverse();
    return Transform(cameraFromWorld, worldFromCamera);
}
