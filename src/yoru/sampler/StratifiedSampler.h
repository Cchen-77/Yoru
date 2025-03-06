#pragma once
#include "Sampler.h"
#include "utils/RNG.h"
class StratifiedSampler : public Sampler {
public:
    StratifiedSampler(int xPixelSamples, int yPixelSamples, bool jitter = true, int seed = 0)
        : xPixelSamples(xPixelSamples), yPixelSamples(yPixelSamples), jitter(jitter), seed(seed) {}
    virtual int SamplesPerPixel() const override;

    virtual void StartPixel(Point2i pixel, int sampleIndex, int dimension = 0) override;
    virtual double Get1D() override;
    virtual Point2 Get2D() override;
    virtual Point2 GetPixel2D() override;

    virtual std::shared_ptr<Sampler> Clone() const;

private:
    int xPixelSamples;
    int yPixelSamples;
    bool jitter;
    int seed;
    RNG rng;

    Point2i pixel;
    int sampleIndex;
    int dimension;
};