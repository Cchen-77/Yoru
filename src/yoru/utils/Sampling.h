#pragma once
#include"base/BoundingBox.h"

#include <vector>
inline double SampleLinear(double u, double a, double b) {
    if (u == 0 && a == 0) {
        return 0;
    }
    double x = u * (a + b) / (a + std::sqrt(std::lerp(u, a*a, b*b)));
    return std::min(x, 0x1.fffffffffffffp-1);
}
inline double SampleTent(double u, double r) {
    if (u < .5) {
        return -r + r*SampleLinear(2 * u, 0, 1);
    } else {
        return r * SampleLinear(2 * u - 1, 0, 1);
    }
}
class PiecewiseConstant1D {
public:
    PiecewiseConstant1D() = default;
    PiecewiseConstant1D(const std::vector<double> &func, double min, double max);
    
    double Sample(double u, double *pdf = nullptr, int *offset=nullptr) const;
    double Integral() const;


private:
    std::vector<double> func;
    std::vector<double> cdf;
    double intFunc = 0;
    double min = 0;
    double max = 0;
};

class PiecewiseConstant2D {
public:
    PiecewiseConstant2D() = default;
    PiecewiseConstant2D(const std::vector<std::vector<double>> &table, BBox2 bbox);

    Point2 Sample(Point2 u, double *pdf=nullptr, Point2i *offset=nullptr) const;
    double Integral();

private:
    int nu;
    double intFunc;
    PiecewiseConstant1D pU;
    std::vector<PiecewiseConstant1D> pUConditionedV;
};