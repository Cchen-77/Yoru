#include "BoundingBox.h"

bool BBox3::Intersect(const Ray &ray, double *hitt0, double *hitt1) const {
    double t0 = ray.timeMin, t1 = ray.timeMax;
    for (int i = 0; i < 3; ++i) {
        double invDir = 1. / ray.direction[i];
        double tNear = (pMin[i] - ray.origin[i]) * invDir;
        double tFar = (pMax[i] - ray.origin[i]) * invDir;
        if (tNear > tFar) std::swap(tNear, tFar);
        t0 = tNear > t0 ? tNear : t0;
        t1 = tFar < t1 ? tFar : t1;
        if (t0 > t1) {
            return false;
        }
    }
    if (hitt0) *hitt0 = t0;
    if (hitt1) *hitt1 = t1;
    return true;
}
