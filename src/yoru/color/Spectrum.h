#pragma once
#include "base/Vector.h"
using SampledSpectrum = Vector3;

// TODO: a real spectrum
static constexpr double lambdaMin = 360.;
static constexpr double lambdaMax = 830.;

class SampledWavelengths {
};

// TODO: a real spectrum impl
// // class SampledSpectrum {
//
// };
// class Spectrum {
// public:
//    virtual double operator()(double lambda) const = 0;
//    virtual double maxValue() const = 0;
//    virtual SampledSpectrum sampleSpectrum(SampledWavelengths wavelengths) = 0;
//};

class Spectrum {
public:
    Spectrum(SampledSpectrum rgb) : rgb(rgb){};
    SampledSpectrum sampleSpectrum(SampledWavelengths wavelengths) const{
        return rgb;
    }

private:
    SampledSpectrum rgb;
};
