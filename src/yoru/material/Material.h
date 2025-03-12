#pragma once
#include "BxDF/BxDF.h"
#include "base/Frame.h"

#include "nlohmann/json.hpp"
#include <memory>
class Intersection;
class Material {
public:
    static std::shared_ptr<Material> Create(const nlohmann::json& json);
    virtual std::shared_ptr<BxDF> GetBxDF(const Intersection &its, const SampledWavelengths &lambdas) const = 0;
    virtual void SetShadingFrame(Frame &shFrame) const {};
};