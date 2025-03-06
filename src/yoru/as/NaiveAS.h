#pragma once
#include "AS.h"
#include "primitive/Primitive.h"

#include <vector>
class NaiveAS : public AS {
public:
    NaiveAS(const std::vector<std::shared_ptr<Primitive>> &primtives) : primitives(primitives) {
        for (auto &primitive : primitives) {
            bbox.Union(primitive->BBox());
        }
    };
    virtual BBox3 BBox() const {
        return bbox;
    }
    virtual std::optional<ShapeIntersection> Intersect(const Ray &ray) const {
        std::optional<ShapeIntersection> resSi;
        Ray r = ray;
        for (auto &primitive : primitives) {
            auto si = primitive->Intersect(r);
            if (si.has_value()) {
                resSi = si;
                r.timeMax = si->t;
            }
        }
        return resSi;
    }
    virtual bool IntersectP(const Ray &ray) const {
        for (auto &primitive : primitives) {
            if (primitive->IntersectP(ray)) {
                return true;
            }
        }
        return false;
    }

private:
    BBox3 bbox;
    std::vector<std::shared_ptr<Primitive>> primitives;
};