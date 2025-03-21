#pragma once
#include "Frame.h"

#include <memory>
class Material;
class AreaLight;
struct Intersection {
    Point3 p;
    Frame geoFrame;
    Frame shFrame;
    Vector3 wo;
    Point2 uv;

    Vector3 dpdu, dpdv;

    std::shared_ptr<Material> material;
    std::shared_ptr<AreaLight> light;
};