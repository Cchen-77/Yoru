#include "AreaLight.h"

#include "shape/Shape.h"

DiffuseAreaLight::DiffuseAreaLight(std::shared_ptr<Shape> shape, Spectrum radiance) : AreaLight(shape), radiance(radiance) {
}

std::optional<LightLiSample> DiffuseAreaLight::SampleLi(const Intersection &its, Point2 u, const SampledWavelengths &lambdas) {
    auto ss = shape->Sample({its.p, its.geoFrame.n, its.shFrame.n}, u);
    if (ss.has_value()) {
        LightLiSample ls{};
        ls.L = radiance.sampleSpectrum(lambdas);
        ls.p = ss->p;
        Vector3 d = (ls.p - its.p);
        ls.wi = Normalized(d);
        ls.pdf = ss->pdf;
        return ls;
    } else {
        return std::nullopt;
    }
}

SampledSpectrum DiffuseAreaLight::L(const Intersection &its, const SampledWavelengths &lambdas) {
    return radiance.sampleSpectrum(lambdas);
}

double DiffuseAreaLight::PDFLi(const Intersection &its, Vector3 wi) {
    return shape->PDF({its.p, its.geoFrame.n, its.shFrame.n}, wi);
}

SampledSpectrum DiffuseAreaLight::Le(const Ray &r, const SampledWavelengths &lambdas) {
    return 0.;
}
