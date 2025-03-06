#pragma once
#include "Filter.h"
class TriangleFilter : public Filter {
public:
    TriangleFilter(Vector2 radius) : radius(radius){};
    virtual Vector2 Radius() const {
        return radius;
    }
    virtual double Evaluate(const Point2 &p) const {
        return std::max(0., radius.x - std::abs(p.x)) * std::max(0., radius.y - std::abs(p.y));
    }
    virtual double Integral() const {
        return radius.x * radius.x * radius.y * radius.y;
    }
    virtual FilterSample Sample(Point2 u) const {
        Point2 p = Point2(SampleTent(u[0], radius.x), SampleTent(u[1], radius.y));
        FilterSample fs;
        fs.weight = Evaluate(p);
        fs.p = p;
        return fs;
    }

private:
    Vector2 radius;
};