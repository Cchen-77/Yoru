#include "Integrator.h"
#include "utils/Parallelism.h"

CameraSample Integrator::GetCamereSample(std::shared_ptr<Sampler> sampler, Point2 pPixel, std::shared_ptr<Filter> filter) {
    CameraSample cs;
    auto fs = filter->Sample(sampler->GetPixel2D());
    cs.pFilm = pPixel + fs.p + Vector2(.5, .5);
    cs.filterWeight = fs.weight;
    return cs;
}

Integrator::Integrator(std::shared_ptr<Camera> camera, std::shared_ptr<AS> as, std::shared_ptr<Sampler> samplerPrototype) : camera(camera), as(as), samplerPrototype(samplerPrototype) {
}

void Integrator::Render() {
    BBox2i pixelBounding = camera->film->GetPixelBounding();
    int spp = samplerPrototype->SamplesPerPixel();
    ParrallelFor2D(pixelBounding, [this,spp](BBox2i tile) {
        for (int x = tile.pMin.x; x < tile.pMax.x; ++x) {
            for (int y = tile.pMin.y; y < tile.pMax.y; ++y) {
                Point2i pPixel(x, y);
                for (int sampleIndex = 0; sampleIndex < spp; ++sampleIndex) {
                    RenderPerPixel(pPixel, sampleIndex,samplerPrototype.get());
                }
            }
        }
    });
}

void Integrator::RenderPerPixel(Point2i pixel, int sampleIndex, const Sampler *samplerPrototype) {
    
}
