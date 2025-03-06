#pragma once
#include "Sampler.h"

#include "utils/RNG.h"
class IndependentSampler : public Sampler {
public:
    IndependentSampler(int spp, uint64_t seed = 0) : spp(spp), seed(seed){};
    virtual int SamplesPerPixel() const override;

    virtual void StartPixel(Point2i pixel, int sampleIndex, int dimension = 0) override;
    virtual double Get1D() override;
    virtual Point2 Get2D() override;
    virtual Point2 GetPixel2D() override;

    virtual std::shared_ptr<Sampler> Clone() const;

private:
    RNG rng;
    uint64_t seed;
    int spp;
};