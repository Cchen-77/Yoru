#pragma once
#include "base/Ray.h"
#include "base/Vector.h"
#include "color/Spectrum.h"

#include "nlohmann/json.hpp"
#include <memory>
struct Intersection;
struct LightLiSample {
    SampledSpectrum L;
    Point3 p;
    Vector3 wi;
    double pdf;

    bool isDeltaDirection;
    bool isDeltaPosition;
};
class Light {
public:
    static std::shared_ptr<Light> Create(const nlohmann::json &json);
    virtual std::optional<LightLiSample> SampleLi(const Intersection &its, Point2 u, const SampledWavelengths &lambdas) const = 0;
    virtual double PDFLi(const Intersection &its, Vector3 wi) const = 0;
    virtual SampledSpectrum L(const Intersection &its, const SampledWavelengths &lambdas) const = 0;
    virtual SampledSpectrum Le(const Ray &r, const SampledWavelengths &lambdas) const = 0;
};