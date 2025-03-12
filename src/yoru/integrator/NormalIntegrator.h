#pragma once
#include "Integrator.h"
class NormalIntegrator : public Integrator {
public:
    NormalIntegrator(std::shared_ptr<Camera> camera, std::shared_ptr<AS> as, std::shared_ptr<Sampler> samplerPrototype);

    virtual void RenderPerPixel(Point2i pixel, int sampleIndex, const Sampler *samplerPrototype);
};