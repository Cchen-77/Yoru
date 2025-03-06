#pragma once
#include "base/BoundingBox.h"
#include "shape/Shape.h"

#include <optional>
class AS {
public:
    virtual BBox3 BBox() const = 0;
    virtual std::optional<ShapeIntersection> Intersect(const Ray &ray) const = 0;
    virtual bool IntersectP(const Ray &ray) const = 0;
};