#pragma once
#include "Shape.h"
#include "base/Common.h"
class Sphere : public Shape {
public:
    Sphere(const Transform &rfo, double radius);

    virtual BBox3 BBox() const;
    virtual std::optional<ShapeIntersection> Intersect(const Ray &ray) const;
    virtual double Area() const;

    virtual std::optional<ShapeSample> Sample(Point2 u) const;
    virtual double PDF(const Intersection &intr) const;

    virtual std::optional<ShapeSample> Sample(const ShapeSampleContext &ctx, Point2 u) const;
    virtual double PDF(const ShapeSampleContext &ctxt,const Intersection& its) const;




private:

    double radius;
};