#pragma once
#include "Eigen/Dense"
#include "BoundingBox.h"
#include "Intersection.h"
#include "Vector.h"
#include "Ray.h"
class Transform {
public:
    Transform()
        : m_transform(Eigen::Matrix4d::Identity()), m_inverse(Eigen::Matrix4d::Identity()) {}
    Transform(const Eigen::Matrix4d &mat);
    Transform(double mat[4][4]);
    Transform(const Eigen::Matrix4d &transform, const Eigen::Matrix4d &inv) : m_transform(transform), m_inverse(inv){};

    Transform Inverse() const {
        return Transform(m_inverse, m_transform);
    }

    Transform operator*(const Transform &other) const;

    Point3 operator()(const Point3 &p) const;
    Vector3 operator()(const Vector3 &v) const;
    Ray operator()(const Ray &r) const;
    BBox3 operator()(const BBox3 &bbox) const;
    Intersection operator()(const Intersection &its) const;

    Eigen::Matrix4d m_transform;
    Eigen::Matrix4d m_inverse;

public:
    static Transform Translation(double tx, double ty, double tz);

    static Transform Scale(double scale);
    static Transform Scale(double sx, double sy, double sz);

    static Transform RotateX(double theta);
    static Transform RotateY(double theta);
    static Transform RotateZ(double theta);

    static Transform Rotate(double cosTheta, double sinTheta, Vector3 axis);
    static Transform Rotate(double theta, Vector3 axis);

    static Transform LookAt(Point3 eye, Point3 center, Vector3 up);

    static Transform Orthographic(double zNear, double zFar);

    static Transform Perspective(double fov, double zNear, double zFar);
};