#pragma once
#include "base/BoundingBox.h"
#include "base/Ray.h"
#include "shape/Shape.h"

#include <optional>
class Primitive {
public:
    virtual BBox3 BBox() const = 0;
    virtual std::optional<ShapeIntersection> Intersect(const Ray &r) const = 0;
    virtual bool IntersectP(const Ray &r) const = 0;
};