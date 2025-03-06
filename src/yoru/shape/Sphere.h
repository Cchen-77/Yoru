#pragma once
#include "Shape.h"
#include "base/Common.h"
class Sphere : public Shape {
public:
    Sphere(const Transform &rfo, double radius);

    virtual BBox3 BBox() const override;
    virtual std::optional<ShapeIntersection> Intersect(const Ray &ray) const override;
    virtual double Area() const override;

    virtual std::optional<ShapeSample> Sample(Point2 u) const override;
    virtual double PDF(const Intersection &intr) const override;

    virtual std::optional<ShapeSample> Sample(const ShapeSampleContext &ctx, Point2 u) const override;

private:

    double radius;
};