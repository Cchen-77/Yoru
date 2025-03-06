#pragma once
#include "base/Ray.h"
#include "base/Transform.h"
#include "base/BoundingBox.h"
#include "color/Spectrum.h"
#include "film/Film.h"

#include <optional>

enum class RenderSpace {
    Camera,
    CameraWorld,
    World
};

// TODO: time,Len
struct CameraSample {
    Point2 pFilm;
    double filterWeight;
};

struct CameraBaseTransform {
    Transform worldFromCamera;
    Transform renderFromCamera;
    Transform renderFromWorld;
};

struct CameraBaseParameters {
    RenderSpace renderSpace;
    Transform worldFromCamera;
    std::shared_ptr<Film> film;
};

class Camera {
public:
    Camera(const CameraBaseParameters &baseParams);
    virtual std::optional<Ray> GenerateRay(const CameraSample &sample, const SampledWavelengths &lambdas) const = 0;
    CameraBaseTransform baseTransform;
    std::shared_ptr<Film> film;
};

class ProjectiveCamera : public Camera {
public:
    ProjectiveCamera(const CameraBaseParameters &baseParams, Transform screenFromCamera, BBox2 screenWindow);

protected:
    Transform screenFromCamera;
    Transform cameraFromRaster, rasterFromCamera;
};

class OrthographicCamera : public ProjectiveCamera {
public:
    OrthographicCamera(const CameraBaseParameters &baseParams, double zNear, double zFar, BBox2 screenWindow);
    virtual std::optional<Ray> GenerateRay(const CameraSample &sample, const SampledWavelengths &lambdas) const;
};

class PerspectiveCamera : public ProjectiveCamera {
public:
    PerspectiveCamera(const CameraBaseParameters &baseParams, double fov, double zNear, double zFar, BBox2 screenWindow);
    virtual std::optional<Ray> GenerateRay(const CameraSample &sample, const SampledWavelengths &lambdas) const;
};