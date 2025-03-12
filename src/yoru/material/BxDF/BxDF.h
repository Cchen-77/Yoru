#pragma once
#include "base/Vector.h"
#include "color/Spectrum.h"
#include "utils/Util.h"

#include <optional>
enum class TransportMode {
    Raidance,
    Importance
};
enum BxDFFlag {
    BxDFFLAG_Unset = 0,
    BxDFFlag_Reflection = 1,
    BxDFFlag_Transmission = 1 << 1,
    BxDFFlag_Diffuse = 1 << 2,
    BxDFFlag_Glossy = 1 << 3,
    BxDFFlag_Specular = 1 << 4
};

struct BxDFSample {
    SampledSpectrum f;
    Vector3 wi;
    double pdf;
    BxDFFlag flag;
};
enum BxDFReflTransFlag {
    BxDFReflTransFlag_Reflection = 1,
    BxDFReflTransFlag_Transmission = 1 << 1,
    BxDFReflTransFlag_All = BxDFReflTransFlag_Reflection | BxDFReflTransFlag_Transmission
};

class BxDF {
public:
    BxDF() = default;
    virtual SampledSpectrum Eval(Vector3 wo, Vector3 wi, TransportMode transportMode = TransportMode::Raidance) const = 0;
    virtual std::optional<BxDFSample> SampleBxDF(Vector3 wo, double uc, Point2 u, BxDFReflTransFlag sampleFlag) const = 0;
    virtual double PDF(Vector3 wo, Vector3 wi, BxDFReflTransFlag sampleFlag) const = 0;
};