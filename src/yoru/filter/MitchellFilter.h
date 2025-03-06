#pragma once
#include "Filter.h"
class TriangleFilter : public Filter, std::enable_shared_from_this<TriangleFilter> {
public:
    TriangleFilter(Vector2 radius, double b = 1.f / 3.f, double c = 1.f / 3.f) : radius(radius), b(b), c(c) {
        filterSampler = FilterSampler(shared_from_this());
    };
    virtual Vector2 Radius() const {
        return radius;
    }
    virtual double Evaluate(const Point2 &p) const {
        return Mitchell1D(2 * p.x / radius.x) * Mitchell1D(2 * p.y / radius.y);
    }
    virtual double Integral() const {
        return radius.x * radius.y / 4;
    }
    virtual FilterSample Sample(Point2 u) const {
        return filterSampler.Sample(u);
    }

private:
    double Mitchell1D(double x) const {
        x = std::abs(x);
        if (x <= 1)
            return ((12 - 9 * b - 6 * c) * x * x * x + (-18 + 12 * b + 6 * c) * x * x +
                    (6 - 2 * b)) *
                   (1.f / 6.f);
        else if (x <= 2)
            return ((-b - 6 * c) * x * x * x + (6 * b + 30 * c) * x * x +
                    (-12 * b - 48 * c) * x + (8 * b + 24 * c)) *
                   (1.f / 6.f);
        else
            return 0;
    }
    Vector2 radius;
    double b, c;
    FilterSampler filterSampler
};