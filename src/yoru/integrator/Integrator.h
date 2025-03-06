#pragma once
#include "as/AS.h"
#include "camera/Camera.h"
#include "color/Spectrum.h"
#include "filter/Filter.h"
#include "sampler/Sampler.h"
class Integrator {
public:
    static CameraSample GetCamereSample(std::shared_ptr<Sampler> sampler, Point2 pPixel, std::shared_ptr<Filter> filter);

    Integrator(std::shared_ptr<Camera> camera, std::shared_ptr<AS> as, std::shared_ptr<Sampler> samplerPrototype);

    void Render();

    virtual void RenderPerPixel(Point2i pixel,int sampleIndex,const Sampler* samplerPrototype);

protected:
    std::shared_ptr<Camera> camera;
    std::shared_ptr<AS> as;
    std::shared_ptr<Sampler> samplerPrototype;
};