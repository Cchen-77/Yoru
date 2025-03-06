#include "utils/Parallelism.h"

#include "as/NaiveAS.h"
#include "camera/Camera.h"
#include "film/Film.h"
#include "sampler/Sampler.h"
#include "sampler/IndependentSampler.h"
#include "integrator/Integrator.h"
#include "primitive/Primitive.h"
#include "primitive/GeometricPrimitive.h"
#include "shape/Shape.h"
#include "shape/Sphere.h"

int main() {
    Parallelism::Config config;
    config.nThreads = 8;
    Parallelism::Init(config);

    FilmBaseParameters filmbp;
    filmbp.fileName = "result";
    filmbp.fullResolution = {1024, 1024};
    filmbp.pixelBounding = BBox2i({0, 0}, filmbp.fullResolution);
    auto film = std::make_shared<RGBFilm>(filmbp);

    CameraBaseParameters camerabp;
    camerabp.film = film;
    camerabp.renderSpace = RenderSpace::Camera;
    camerabp.worldFromCamera = Transform::LookAt({0, 0, 1}, {0, 0, 0}, {0, 1, 0});

    auto camera = std::make_shared<PerspectiveCamera>(camerabp);

    auto sampler = std::make_shared<IndependentSampler>(1);

    auto sphere = std::make_shared<Sphere>(Transform(), 1);

    auto spherePrimitvie = std::make_shared<Primitive>(sphere, nullptr, nullptr);

    std::vector<std::shared_ptr<Primitive>> primitives;
    primitives.push_back(spherePrimitvie);

    auto as = std::make_shared<NaiveAS>(primitives);

    Integrator integrator(camera, as, sampler);

    integrator.Render();
    
    film->WriteImage();
}