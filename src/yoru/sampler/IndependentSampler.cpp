#include "IndependentSampler.h"

int IndependentSampler::SamplesPerPixel() const {
    return spp;
}

void IndependentSampler::StartPixel(Point2i pixel, int sampleIndex, int dimension) {
    rng.SetSequence(Hash(pixel), seed);
    rng.Advance(sampleIndex*65536ull+dimension);
}

double IndependentSampler::Get1D() {
    return rng.Uniform<double>();
}

Point2 IndependentSampler::Get2D() {
    return {rng.Uniform<double>(), rng.Uniform<double>()};
}

Point2 IndependentSampler::GetPixel2D() {
    return Get2D();
}

std::shared_ptr<Sampler> IndependentSampler::Clone() const {
    return std::shared_ptr<Sampler>(new IndependentSampler(spp, seed));
}
