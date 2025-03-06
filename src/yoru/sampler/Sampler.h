#pragma once
#include "base/Vector.h"

#include <memory>
class Sampler {
public:
    virtual int SamplesPerPixel() const = 0;

    virtual void StartPixel(Point2i pixel, int sampleIndex,int dimension = 0) = 0;
    virtual double Get1D() = 0;
    virtual Point2 Get2D() = 0;
    virtual Point2 GetPixel2D() = 0;

    virtual std::shared_ptr<Sampler> Clone() const = 0;
};