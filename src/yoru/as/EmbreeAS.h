#pragma once
#include "AS.h"
#include "primitive/Primitive.h"

#include <memory>
#include "embree4/rtcore.h"
struct YoruRTCRayHit : public RTCRayHit {
    std::optional<ShapeIntersection> si;
};
class EmbreeAS : public AS {
public:
    EmbreeAS(const std::vector<std::shared_ptr<Primitive>> &primtives);
    ~EmbreeAS();
    virtual BBox3 BBox() const override;
    virtual std::optional<ShapeIntersection> Intersect(const Ray &ray) const override;
    virtual bool IntersectP(const Ray &ray) const override;

private:
    RTCGeometry geometryFromPrimitive(Primitive* primitive) const;
    RTCRay rayFromRay(const Ray &ray) const;

    static void errorFunction(void *userPtr, enum RTCError error, const char *str);
    static void boundsFunction(const RTCBoundsFunctionArguments *args);
    static void intersectFunction(const RTCIntersectFunctionNArguments *args);
    static void occludedFunction(const RTCOccludedFunctionNArguments *args);

private:
    std::vector<std::shared_ptr<Primitive>> primitives;

    RTCDevice device;
    RTCScene scene;
};