#pragma once
#include "Material.h"
#include "texture/Textures.h"
class DiffuseMaterial : public Material {
public:
    DiffuseMaterial(std::shared_ptr<SpectrumTexture> albedoTexture);
    virtual std::shared_ptr<BxDF> GetBxDF(const Intersection &its, const SampledWavelengths &lambdas) const;

private:
    std::shared_ptr<SpectrumTexture> albedoTexture;
};