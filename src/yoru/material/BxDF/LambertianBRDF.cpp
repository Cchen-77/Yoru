#include "LambertianBRDF.h"
#include "base/Common.h"
#include "utils/Util.h"
#include "utils/Sampling.h"
SampledSpectrum LambertianBRDF::Eval(Vector3 wo, Vector3 wi, TransportMode transportMode) const {
    if (Dot(wo, wi) < 0) {
        return 0.;
    }
    return albedo * M_INVPI;
}

std::optional<BxDFSample> LambertianBRDF::SampleBxDF(Vector3 wo, double uc, Point2 u, BxDFReflTransFlag sampleFlag) const {
    if (!(sampleFlag & BxDFReflTransFlag_Reflection)) {
        return std::nullopt;
    }
    Vector3 wi = SampleHemisphere(u);
    BxDFSample sample;
    sample.f = albedo * M_INVPI;
    sample.wi = wi;
    sample.pdf = M_INVPI / 2;
    return sample;
}

double LambertianBRDF::PDF(Vector3 wo, Vector3 wi, BxDFReflTransFlag sampleFlag) const {
    if (!(sampleFlag & BxDFReflTransFlag_Reflection)) {
        return 0.;
    }
    if (Dot(wo, wi) < 0.) {
        return 0;
    }
    return M_INVPI / 2;
}
