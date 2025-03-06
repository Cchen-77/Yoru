#pragma once
#include "base/BoundingBox.h"
#include "base/Intersection.h"
#include "base/Transform.h"
#include "base/Ray.h"

#include <optional>
struct ShapeIntersection {
    Intersection its;
    double t;
};
struct ShapeSample {
    Point3 p;
    Vector3 geoNormal;
    double pdf;
    Point2 uv;
};
struct ShapeSampleContext {
};
class Shape {
public:
    Shape(const Transform &rfo) : renderFromObject(rfo){};
    virtual BBox3 BBox() const = 0;
    virtual std::optional<ShapeIntersection> Intersect(const Ray &ray) const = 0;
    virtual bool IntersectP(const Ray &ray) const {
        return Intersect(ray).has_value();
    }
    virtual double Area() const = 0;

    virtual std::optional<ShapeSample> Sample(Point2 u) const = 0;
    virtual double PDF(const Intersection &its) const = 0;

    virtual std::optional<ShapeSample> Sample(const ShapeSampleContext &ctx, Point2 u) const { return std::nullopt; }

protected:
    Transform renderFromObject;
};