#include "Textures.h"

std::shared_ptr<FloatTexture> FloatTexture::Create(const nlohmann::json &json) {
    return std::shared_ptr<FloatTexture>();
}

std::shared_ptr<SpectrumTexture> SpectrumTexture::Create(const nlohmann::json &json) {
    return std::shared_ptr<SpectrumTexture>();
}

FloatConstantTexture::FloatConstantTexture(double c) : c(c) {
}

double FloatConstantTexture::Eval(const Intersection &its) {
    return c;
}

SpectrumConstantTexture::SpectrumConstantTexture(SampledSpectrum c) : c(c) {
}

SampledSpectrum SpectrumConstantTexture::Eval(const Intersection &its, const SampledWavelengths &lambdas) {
    return c;
}
