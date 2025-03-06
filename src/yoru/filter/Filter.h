#pragma once
#include "base/Vector.h"
#include "utils/Sampling.h"

#include <memory>
struct FilterSample {
    Point2 p;
    double weight;
};
class Filter {
public:
    virtual Vector2 Radius() const = 0;
    virtual double Evaluate(const Point2 &p) const = 0;
    virtual double Integral() const = 0;
    virtual FilterSample Sample(Point2 u) const = 0;
};
// TODO: now a temp impl
class FilterSampler {
public:
    FilterSampler() = default;
    FilterSampler(std::shared_ptr<Filter> filter);
    FilterSample Sample(Point2 u) const;

private:
    PiecewiseConstant2D distrib;
    std::shared_ptr<Filter> filter;

    std::vector<std::vector<double>> table;
};
