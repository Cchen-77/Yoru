#include "NormalIntegrator.h"

#include "filter/Filter.h"

NormalIntegrator::NormalIntegrator(std::shared_ptr<Camera> camera, std::shared_ptr<AS> as, std::shared_ptr<Sampler> samplerPrototype) : Integrator(camera, as, samplerPrototype) {
}

void NormalIntegrator::RenderPerPixel(Point2i pixel, int sampleIndex, const Sampler *samplerPrototype) {
    CameraSample cs;
    cs.filterWeight = 1;
    cs.pFilm = {pixel.x + 0.5, pixel.y + 0.5};

    auto ray = camera->GenerateRay(cs, {});
    if (ray) {
        auto si = as->Intersect(ray.value());
        if (si) {
            SampledSpectrum result = si->its.geoFrame.n * 0.5 + Vector3(0.5, 0.5, 0.5);
            camera->film->AddSample(pixel, result, {}, 1.);
        }
    }
}
