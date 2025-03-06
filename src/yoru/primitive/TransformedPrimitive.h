#pragma once
#include "Primitive.h"
#include "base/Transform.h"

#include <memory>
class TransformedPrimtive : public Primitive {
public:
    TransformedPrimtive(Transform renderFromPrimitive, std::shared_ptr<Primitive> originPrimitive);
    virtual BBox3 BBox() const override;
    virtual std::optional<ShapeIntersection> Intersect(const Ray &r) const override;
    virtual bool IntersectP(const Ray &r) const override;

protected:
    Transform renderFromPrimitive;
    std::shared_ptr<Primitive> originPrimitive;
};