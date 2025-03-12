#include "DiffuseMaterial.h"

#include "BxDF/LambertianBRDF.h"
DiffuseMaterial::DiffuseMaterial(std::shared_ptr<SpectrumTexture> albedoTexture) : albedoTexture(albedoTexture) {
}
std::shared_ptr<BxDF> DiffuseMaterial::GetBxDF(const Intersection &its, const SampledWavelengths &lambdas) const {
    SampledSpectrum albedo = albedoTexture->Eval(its, lambdas);
    auto bxdf = std::make_shared<LambertianBRDF>(albedo);
    return bxdf;
}