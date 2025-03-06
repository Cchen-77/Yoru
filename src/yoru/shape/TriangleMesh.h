#pragma once
#include "Shape.h"
#include "utils/BufferCache.h"

#include <memory>
struct TriangleIntersection {
    double b0, b1, b2;
    double t;
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
};
class Triangle : public Shape {
public:
    Triangle(int meshIndex, int faceIndex);
    virtual BBox3 BBox() const;
    virtual std::optional<ShapeIntersection> Intersect(const Ray &ray);
    virtual double Area();

    virtual std::optional<ShapeSample> Sample(Point2 u);
    virtual double PDF(const Intersection &intr);

    static std::vector<std::shared_ptr<TriangleMesh>> allMeshs;

public:
    static std::optional<TriangleIntersection> IntersectTriangle(const Ray &ray, Point3 p0, Point3 p1, Point3 p2);

private:
    int meshIndex;
    int faceIndex;

};

