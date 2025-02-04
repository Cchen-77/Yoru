#pragma once
#include "base/Vector.h"
using SampledSpectrum = Vector3;

// TODO: a real spectrum
static constexpr double lambdaMin = 360.;
static constexpr double lambdaMax = 830.;

// class SampledSpectrum {
//
// };
class SampledWavelengths {
};

class Spectrum {
public:
    virtual double operator()(double lambda) const = 0;
    virtual double maxValue() const = 0;
    virtual SampledSpectrum sampleSpectrum(SampledWavelengths wavelengths) = 0;
};