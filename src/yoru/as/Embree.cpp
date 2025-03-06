#include "EmbreeAS.h"

EmbreeAS::EmbreeAS(const std::vector<Primitive> &primtives) : primitives(primtives) {

}

BBox3 EmbreeAS::BBox() const {
    return BBox3();
}

std::optional<ShapeIntersection> EmbreeAS::Intersect(const Ray &ray) const {
    return std::optional<ShapeIntersection>();
}

bool EmbreeAS::IntersectP(const Ray &ray) const {
    return false;
}
