#include "utils/Parallelism.h"

#include "as/NaiveAS.h"
#include "as/EmbreeAS.h"
#include "camera/Camera.h"
#include "film/Film.h"
#include "sampler/Sampler.h"
#include "sampler/IndependentSampler.h"
#include "integrator/Integrator.h"
#include "integrator/NormalIntegrator.h"
#include "primitive/Primitive.h"
#include "primitive/GeometricPrimitive.h"
#include "primitive/TransformedPrimitive.h"
#include "shape/Shape.h"
#include "shape/Sphere.h"
#include "shape/TriangleMesh.h"

#include <iostream>
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
    camerabp.worldFromCamera = Transform::LookAt({0, 2, 4}, {0, 1, 0}, {0, 1, 0}).Inverse();

    auto camera = std::make_shared<PerspectiveCamera>(camerabp, 45., 1e-4, 1000, BBox2({-1, -1}, {1, 1}));

    auto sampler = std::make_shared<IndependentSampler>(1);

    std::vector<std::shared_ptr<Primitive>> primitives;
    auto sphere = std::make_shared<Sphere>(camera->baseTransform.renderFromWorld, 1);
    auto bunnyMeshIndex = TriangleMesh::CreateTriangleMeshFromObj(camera->baseTransform.renderFromWorld, "C:/Projects/Zoltraak/resources/models/bunny.obj");
    auto triangles = TriangleMesh::GenerateTriangles(bunnyMeshIndex);
    for (auto &triangle : triangles) {
        primitives.push_back(std::make_shared<GeometricPrimitve>(triangle, nullptr, nullptr));
    }

   /* auto spherePrimitvie = std::make_shared<GeometricPrimitve>(sphere, nullptr, nullptr);
    auto transformedPrimitve = std::make_shared<TransformedPrimtive>(Transform::Translation(-1, 0, -1), spherePrimitvie);

    primitives.push_back(spherePrimitvie);
    primitives.push_back(transformedPrimitve);*/

    auto as = std::make_shared<EmbreeAS>(primitives);

    NormalIntegrator integrator(camera, as, sampler);

    integrator.Render();

    film->WriteImage();
}