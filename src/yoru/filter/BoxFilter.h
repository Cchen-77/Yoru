#pragma once
#include "Filter.h"
class BoxFilter : public Filter {
public:
    BoxFilter(Vector2 radius) : radius(radius){};
    virtual Vector2 Radius() const {
        return radius;
    }
    virtual double Evaluate(const Point2 &p) const {
        if (std::abs(p.x) > radius.x || std::abs(p.y) > radius.y) {
            return 0.;
        }
        return 1.;
    }
    virtual double Integral() const {
        return 4 * radius.x * radius.y;
    }
    virtual FilterSample Sample(Point2 u) const {
        Point2 p = Point2(std::lerp(-radius.x, -radius.y, u[0]), std::lerp(-radius.y, radius.y, u[1]));
        FilterSample fs;
        fs.weight = 1.;
        fs.p = p;
        return fs;
    }

private:
    Vector2 radius;
};