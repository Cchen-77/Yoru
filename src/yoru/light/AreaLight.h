#pragma once
#include "Light.h"

class Shape;
class AreaLight : public Light {
public:
    AreaLight(std::shared_ptr<Shape> shape) : shape(shape){};
    virtual std::optional<LightLiSample> SampleLi(const Intersection &its, Point2 u, const SampledWavelengths &lambdas) const = 0;
    virtual SampledSpectrum L(const Intersection &its, const SampledWavelengths &lambdas) const = 0;
    virtual double PDFLi(const Intersection &its, Vector3 wi) const = 0;
    virtual SampledSpectrum Le(const Ray &r, const SampledWavelengths &lambdas) const = 0;

protected:
    std::shared_ptr<Shape> shape;
};

class DiffuseAreaLight : public AreaLight {
public:
    DiffuseAreaLight(std::shared_ptr<Shape> shape, Spectrum radiance);
    virtual std::optional<LightLiSample> SampleLi(const Intersection &its, Point2 u, const SampledWavelengths &lambdas);
    virtual SampledSpectrum L(const Intersection &its, const SampledWavelengths &lambdas);
    virtual double PDFLi(const Intersection &its, Vector3 wi);
    virtual SampledSpectrum Le(const Ray &r, const SampledWavelengths &lambdas);

protected:
    Spectrum radiance;
};