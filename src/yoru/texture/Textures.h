#pragma once
#include "color/Spectrum.h"
#include "nlohmann/json.hpp"

#include <memory>
struct Intersection;
class FloatTexture {
public:
    static std::shared_ptr<FloatTexture> Create(const nlohmann::json &json);
    virtual double Eval(const Intersection &its) = 0;
};
class SpectrumTexture {
public:
    static std::shared_ptr<SpectrumTexture> Create(const nlohmann::json &json);
    virtual SampledSpectrum Eval(const Intersection &its, const SampledWavelengths &lambdas) = 0;
};

class FloatConstantTexture : public FloatTexture {
public:
    FloatConstantTexture(double c);
    virtual double Eval(const Intersection &its);

private:
    double c;
};

class SpectrumConstantTexture : public SpectrumTexture {
public:
    SpectrumConstantTexture(SampledSpectrum c);
    virtual SampledSpectrum Eval(const Intersection &its, const SampledWavelengths &lambdas);

private:
    SampledSpectrum c;
};