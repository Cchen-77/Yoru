#include "Camera.h"

Camera::Camera(const CameraBaseParameters &baseParams) : film(baseParams.film) {
    Transform &worldFromCamera = baseTransform.worldFromCamera;
    Transform &renderFromCamera = baseTransform.renderFromCamera;
    Transform &renderFromWorld = baseTransform.renderFromWorld;

    worldFromCamera = baseParams.worldFromCamera;
    switch (baseParams.renderSpace) {
        case RenderSpace::Camera: {
            renderFromWorld = worldFromCamera.Inverse();
            break;
        }
        case RenderSpace::CameraWorld: {
            Point3 pCamera = worldFromCamera(Point3{0, 0, 0});
            renderFromWorld = Transform::Translation(-pCamera.x, -pCamera.y, -pCamera.z);
            break;
        }
        case RenderSpace::World: {
            renderFromWorld = Transform();
            break;
        }
    }
    renderFromCamera = renderFromWorld * worldFromCamera;
}

ProjectiveCamera::ProjectiveCamera(const CameraBaseParameters &baseParams, Transform screenFromCamera, BBox2 screenWindow)
    : Camera(baseParams),
      screenFromCamera(screenFromCamera) {
    // NDC: [0,1]^3
    Transform NDCFromScreen =
        Transform::Scale(1. / (screenWindow.pMax.x - screenWindow.pMin.x), 1. / (screenWindow.pMax.y - screenWindow.pMin.y), 1.) *
        Transform::Translation(-screenWindow.pMin.x, -screenWindow.pMin.y, 0);
    Point2i resolution = film->GetFullResolution();
    Transform rasterFromNDC = Transform::Scale(resolution.x, resolution.y, 1.);

    rasterFromCamera = rasterFromNDC * NDCFromScreen * screenFromCamera;
    cameraFromRaster = rasterFromCamera.Inverse();
}

OrthographicCamera::OrthographicCamera(const CameraBaseParameters &baseParams, double zNear, double zFar, BBox2 screenWindow)
    : ProjectiveCamera(baseParams, Transform::Orthographic(zNear, zFar), screenWindow) {
}

std::optional<Ray> OrthographicCamera::GenerateRay(const CameraSample &sample, const SampledWavelengths &lambdas) const {
    Point3 pFilm = Point3(sample.pFilm.x, sample.pFilm.y, 0);
    Point3 pCamera = cameraFromRaster(pFilm);

    Ray r = Ray(pCamera, {0, 0, -1});

    return baseTransform.renderFromCamera(r);
}

PerspectiveCamera::PerspectiveCamera(const CameraBaseParameters &baseParams, double fov, double zNear, double zFar, BBox2 screenWindow)
    : ProjectiveCamera(baseParams, Transform::Perspective(fov, zNear, zFar), screenWindow) {
}

std::optional<Ray> PerspectiveCamera::GenerateRay(const CameraSample &sample, const SampledWavelengths &lambdas) const {
    Point3 pFilm = Point3(sample.pFilm.x, sample.pFilm.y, 0);
    Point3 pCamera = cameraFromRaster(pFilm);

    Ray r = Ray({0, 0, 0}, Normalized(Vector3(pCamera)));

    return baseTransform.renderFromCamera(r);
}
