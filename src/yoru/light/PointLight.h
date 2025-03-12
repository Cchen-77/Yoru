#pragma once
#include "Light.h"
#include "base/Vector.h"
#include "base/Transform.h"
#include "color/Spectrum.h"
class PointLight : public Light {
public:
    PointLight(Transform renderFromLight, Spectrum intensity);
    virtual std::optional<LightLiSample> SampleLi(const Intersection &its, Point2 u, const SampledWavelengths &lambdas) const;
    virtual SampledSpectrum L(const Intersection &its, const SampledWavelengths &lambdas) const;
    virtual double PDFLi(const Intersection &its, Vector3 wi) const;
    virtual SampledSpectrum Le(const Ray &r, const SampledWavelengths &lambdas) const;

private:
    Transform renderFromLight;
    Spectrum intensity;
};