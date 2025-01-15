#include "Eigen/Core"
#include"Vector.h"
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

    Point3 operator*(const Point3 & p) const;
    Vector3 operator*(const Vector3 &v) const;

    Eigen::Matrix4d m_transform;
    Eigen::Matrix4d m_inverse;
};