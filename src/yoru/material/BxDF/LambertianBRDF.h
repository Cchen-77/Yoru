#pragma once
#include "BxDF.h"
class LambertianBRDF : public BxDF {
    SampledSpectrum albedo;

public:
    LambertianBRDF(SampledSpectrum albedo) : albedo(albedo){};
    virtual SampledSpectrum Eval(Vector3 wo, Vector3 wi, TransportMode transportMode = TransportMode::Raidance) const;
    virtual std::optional<BxDFSample> SampleBxDF(Vector3 wo, double uc, Point2 u, BxDFReflTransFlag sampleFlag) const;
    virtual double PDF(Vector3 wo, Vector3 wi, BxDFReflTransFlag sampleFlag) const;
};