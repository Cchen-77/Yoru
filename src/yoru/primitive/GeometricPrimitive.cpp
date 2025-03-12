#include "GeometricPrimitive.h"

GeometricPrimitve::GeometricPrimitve(std::shared_ptr<Shape> shape, std::shared_ptr<Material> material, std::shared_ptr<AreaLight> light)
    : shape(shape), material(material), light(light) {
}

BBox3 GeometricPrimitve::BBox() const {
    return shape->BBox();
}

std::optional<ShapeIntersection> GeometricPrimitve::Intersect(const Ray &r) const {
    auto si = shape->Intersect(r);
    if (si) {
        si->its.material = material;
        si->its.light = light;
        if (material) {
            material->SetShadingFrame(si->its.shFrame);
        }
    }
    return si;
}

bool GeometricPrimitve::IntersectP(const Ray &r) const {
    return shape->IntersectP(r);
}
