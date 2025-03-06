#include "EmbreeAS.h"

#include <iostream>

EmbreeAS::EmbreeAS(const std::vector<std::shared_ptr<Primitive>> &_primtives) : primitives(_primtives) {
    device = rtcNewDevice(NULL);
    if (!device) {
        printf("embree error %d: cannot create device\n", rtcGetDeviceError(device));
    }
    rtcSetDeviceErrorFunction(device, errorFunction, NULL);

    scene = rtcNewScene(device);
    for (auto& primitve : primitives) {
        RTCGeometry geom = geometryFromPrimitive(primitve.get());
        rtcCommitGeometry(geom);
        rtcAttachGeometry(scene, geom);
        rtcReleaseGeometry(geom);
    }
}

EmbreeAS::~EmbreeAS() {
}

BBox3 EmbreeAS::BBox() const {
    return BBox3();
}

std::optional<ShapeIntersection> EmbreeAS::Intersect(const Ray &ray) const {
    YoruRTCRayHit rayhit;
    rayhit.ray = rayFromRay(ray);
    rtcIntersect1(scene, &rayhit);
    return rayhit.si;
}

bool EmbreeAS::IntersectP(const Ray &ray) const {
    RTCRay r = rayFromRay(ray);
    rtcOccluded1(scene, &r);
    return r.tfar > r.tnear;
}

RTCGeometry EmbreeAS::geometryFromPrimitive(Primitive *primitive) const {
    RTCGeometry geom = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_USER);
    rtcSetGeometryUserPrimitiveCount(geom, 1);
    rtcSetGeometryUserData(geom, primitive);
    rtcSetGeometryBoundsFunction(geom, boundsFunction, NULL);
    rtcSetGeometryIntersectFunction(geom, intersectFunction);
    rtcSetGeometryOccludedFunction(geom, occludedFunction);
    return geom;
}

RTCRay EmbreeAS::rayFromRay(const Ray &ray) const {
    RTCRay r{};
    r.org_x = ray.origin[0];
    r.org_y = ray.origin[1];
    r.org_z = ray.origin[2];

    r.dir_x = ray.direction[0];
    r.dir_y = ray.direction[1];
    r.dir_z = ray.direction[2];

    r.tfar = ray.timeMax;
    r.tnear = ray.timeMin;
    
    return r;
}

void EmbreeAS::boundsFunction(const RTCBoundsFunctionArguments *args) {
    Primitive *primtive = static_cast<Primitive *>(args->geometryUserPtr);
    RTCBounds *bounds = args->bounds_o;
    BBox3 bbox = primtive->BBox();
    bounds->lower_x = bbox.pMin[0];
    bounds->lower_y = bbox.pMin[1];
    bounds->lower_z = bbox.pMin[2];

    bounds->upper_x = bbox.pMax[0];
    bounds->upper_y = bbox.pMax[1];
    bounds->upper_z = bbox.pMax[2];
}

void EmbreeAS::intersectFunction(const RTCIntersectFunctionNArguments *args) {
    Primitive *primtive = static_cast<Primitive *>(args->geometryUserPtr);
    assert(args->N == 1);
    if (!args->valid[0]) {
        return;
    }
    YoruRTCRayHit *rayhit = (YoruRTCRayHit *)(args->rayhit);
    auto &ray = rayhit->ray;
    Ray r(Point3(ray.org_x, ray.org_y, ray.org_z), Vector3(ray.dir_x, ray.dir_y, ray.dir_z), ray.tnear, ray.tfar);

    auto si = primtive->Intersect(r);
    if (si.has_value()) {
        rayhit->si = si;
        rayhit->ray.tfar = si->t;
    }
    return;
}

void EmbreeAS::occludedFunction(const RTCOccludedFunctionNArguments *args) {
    Primitive *primtive = static_cast<Primitive *>(args->geometryUserPtr);
    assert(args->N == 1);
    if (!args->valid[0]) {
        return;
    }
    RTCRay *ray = (RTCRay *)args->ray;
    Ray r(Point3(ray->org_x, ray->org_y, ray->org_z), Vector3(ray->dir_x, ray->dir_y, ray->dir_z), ray->tnear, ray->tfar);
    bool hit = primtive->IntersectP(r);

    if (hit) {
        ray->tfar = -1e20;
    }
    return;

}

void EmbreeAS::errorFunction(void *userPtr, RTCError error, const char *str) {
    printf("embree error %d: %s\n", error, str);
}
