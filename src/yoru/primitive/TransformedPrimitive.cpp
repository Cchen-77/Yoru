#include "TransformedPrimitive.h"

TransformedPrimtive::TransformedPrimtive(Transform renderFromPrimitive, std::shared_ptr<Primitive> originPrimitive)
    : renderFromPrimitive(renderFromPrimitive), originPrimitive(originPrimitive) {
}

BBox3 TransformedPrimtive::BBox() const {
    BBox3 bbox = originPrimitive->BBox();
    return renderFromPrimitive(bbox);
}

std::optional<ShapeIntersection> TransformedPrimtive::Intersect(const Ray &ray) const {
    Ray r = (renderFromPrimitive.Inverse())(ray);
    auto si = originPrimitive->Intersect(r);
    if (!si.has_value()) {
        return std::nullopt;
    }
    return ShapeIntersection(renderFromPrimitive(si->its), si->t);
}

bool TransformedPrimtive::IntersectP(const Ray &r) const {
    return originPrimitive->IntersectP((renderFromPrimitive.Inverse())(r));
}
