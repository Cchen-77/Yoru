#pragma once
#include "Shape.h"
#include "utils/BufferCache.h"

#include <memory>
struct TriangleIntersection {
    double b0, b1, b2;
    double t;
};
class Triangle : public Shape {
public:
    Triangle(int meshIndex, int faceIndex);
    virtual BBox3 BBox() const;
    virtual std::optional<ShapeIntersection> Intersect(const Ray &ray) const;
    virtual double Area() const;

    virtual std::optional<ShapeSample> Sample(Point2 u) const;
    virtual double PDF(const Intersection &intr) const;

    virtual std::optional<ShapeSample> Sample(const ShapeSampleContext &ctx, Point2 u) const;
    virtual double PDF(const ShapeSampleContext &ctxt, const Intersection &its) const;

public:
    static std::optional<TriangleIntersection> IntersectTriangle(const Ray &ray, Point3 p0, Point3 p1, Point3 p2);

private:
    int meshIndex;
    int faceIndex;
};
class TriangleMesh {
public:
    TriangleMesh(Transform &renderFromObject,
                 int nTriangles, int nVertices,
                 std::vector<int> indices,
                 std::vector<Point3> vertices,
                 std::vector<Vector3> tangents,
                 std::vector<Vector3> normals,
                 std::vector<Point2> uvs);

    int nTriangles;
    int nVertices;
    const int *indices = nullptr;
    const Point3 *vertices = nullptr;
    const Vector3 *tangents = nullptr;
    const Vector3 *normals = nullptr;
    const Point2 *uvs = nullptr;

public:
    static std::vector<std::shared_ptr<TriangleMesh>> allMeshs;
    static int CreateTriangleMeshFromObj(Transform &renderFromObject, const std::string &obj);
    static std::vector<std::shared_ptr<Triangle>> GenerateTriangles(int meshIndex);
};
