#pragma once
#include "base/Ray.h"
#include "color/Spectrum.h"

#include <optional>
struct CameraSample {
    Point2 pFilm;
    double filterWeight;
};
class Camera {
public:
    virtual std::optional<Ray> GenerateRay(const CameraSample &sample, const SampledWavelengths& lambdas) const;

};