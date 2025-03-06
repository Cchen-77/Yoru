#include "StratifiedSampler.h"

#include "utils/Hash.h"
#include "utils/Util.h"

int StratifiedSampler::SamplesPerPixel() const {
    return xPixelSamples * yPixelSamples;
}

void StratifiedSampler::StartPixel(Point2i _pixel, int _sampleIndex, int _dimension) {
    pixel = _pixel;
    sampleIndex = _sampleIndex;
    dimension = _dimension;
    rng.SetSequence(Hash(pixel), seed);
    rng.Advance(sampleIndex * 65536ull + dimension);
}

double StratifiedSampler::Get1D(){
    uint32_t hash = Hash(pixel,dimension,seed);
    int stratum = PermutationElement(sampleIndex, SamplesPerPixel(), hash);
    
    ++dimension;
    return (stratum + (jitter ? rng.Uniform<double>() : .5)) / SamplesPerPixel();
}

Point2 StratifiedSampler::Get2D() {
    uint32_t hash = Hash(pixel, dimension, seed);
    int stratum = PermutationElement(sampleIndex,SamplesPerPixel(),hash);

    int xStratum = stratum % xPixelSamples, yStratum = stratum / yPixelSamples;
    
    dimension += 2;
    return Point2((xStratum + (jitter ? rng.Uniform<double>() : .5) / xPixelSamples), (yStratum + (jitter ? rng.Uniform<double>() : .5) / yPixelSamples));
}

Point2 StratifiedSampler::GetPixel2D() {
    return Get2D();
}

std::shared_ptr<Sampler> StratifiedSampler::Clone() const {
    return std::shared_ptr<Sampler>(new StratifiedSampler(xPixelSamples, yPixelSamples, jitter, seed));
}
