#include "PointLight.h"

PointLight::PointLight(Transform renderFromLight, Spectrum I) : renderFromLight(renderFromLight), intensity(intensity) {
}

std::optional<LightLiSample> PointLight::SampleLi(const Intersection &its, Point2 u, const SampledWavelengths &lambdas) const {
    LightLiSample ls{};
    ls.isDeltaPosition = true;
    ls.p = renderFromLight(Point3(0, 0, 0));
    Vector3 d = ls.p - its.p;
    ls.wi = Normalized(d);
    ls.L = intensity.sampleSpectrum(lambdas) / d.Length2();
    ls.pdf = 1;
    return ls;
}

// you never need eval a point ligt since it is not hittable
SampledSpectrum PointLight::L(const Intersection &its, const SampledWavelengths &lambdas) const {
    return 0.;
}

double PointLight::PDFLi(const Intersection &its, Vector3 wi) const {
    return 0.;
}

SampledSpectrum PointLight::Le(const Ray &r, const SampledWavelengths &lambdas) const {
    return 0.;
}
