#pragma once
#include "Primitive.h"

#include "light/AreaLight.h"
#include "material/Material.h"
#include "shape/Shape.h"

#include <memory>
class GeometricPrimitve : public Primitive {
public:
    GeometricPrimitve(std::shared_ptr<Shape> shape, std::shared_ptr<Material> material, std::shared_ptr<AreaLight> light);
    virtual BBox3 BBox() const override;
    virtual std::optional<ShapeIntersection> Intersect(const Ray &r) const override;
    virtual bool IntersectP(const Ray &r) const override;

protected:
    std::shared_ptr<Shape> shape;
    std::shared_ptr<Material> material;
    std::shared_ptr<AreaLight> light;
};