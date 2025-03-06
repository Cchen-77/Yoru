#include "TriangleMesh.h"

std::vector<std::shared_ptr<TriangleMesh>> Triangle::allMeshs;

TriangleMesh::TriangleMesh(Transform &renderFromObject,
                           int _nTriangles, int _nVertices,
                           std::vector<int> _indices,
                           std::vector<Point3> _vertices,
                           std::vector<Vector3> _tangents,
                           std::vector<Vector3> _normals,
                           std::vector<Point2> _uvs) : nTriangles(_nTriangles), nVertices(_nVertices) {

    indices = intBufferCache.LookupOrAdd(_indices);

    for (auto &v : _vertices) {
        v = renderFromObject(v);
    }
    vertices = point3BufferCache.LookupOrAdd(_vertices);

    if (_tangents.size()) {
        assert(_tangents.size() == _vertices.size());
        for (auto &s : _tangents) {
            s = renderFromObject(s);
        }
        tangents = vector3BufferCache.LookupOrAdd(_tangents);
    }

    if (_normals.size()) {
        assert(_normals.size() == _vertices.size());
        for (auto &n : _normals) {
            n = renderFromObject(n);
        }
        normals = vector3BufferCache.LookupOrAdd(_normals);
    }

    uvs = point2BufferCache.LookupOrAdd(_uvs);
}

Triangle::Triangle(int meshIndex, int faceIndex) : Shape(Transform()), meshIndex(meshIndex), faceIndex(faceIndex) {
}

BBox3 Triangle::BBox() const {
    std::shared_ptr<TriangleMesh> mesh = allMeshs[meshIndex];
    const int *indices = &(mesh->indices[3 * faceIndex]);
    Point3 p0 = mesh->vertices[indices[0]], p1 = mesh->vertices[indices[1]], p2 = mesh->vertices[indices[2]];
    BBox3 bbox(p0, p1);
    bbox.Union(p2);
    return bbox;
}

std::optional<ShapeIntersection> Triangle::Intersect(const Ray &ray) {
    std::shared_ptr<TriangleMesh> mesh = allMeshs[meshIndex];
    const int *indices = &(mesh->indices[3 * faceIndex]);
    Point3 p0 = mesh->vertices[indices[0]], p1 = mesh->vertices[indices[1]], p2 = mesh->vertices[indices[2]];

    auto ti = IntersectTriangle(ray, p0, p1, p2);
    if (!ti.has_value()) {
        return std::nullopt;
    }

    Intersection its;
    its.p = p0 * ti->b0 + p1 * ti->b1 + p2 * ti->b2;
    its.wo = -ray.direction;

    Vector3 geometryNormal = Normalized(Cross(p1 - p0, p2 - p0));
    its.geoFrame = Frame(geometryNormal);
    its.shFrame = its.geoFrame;

    std::array<Point2, 3> uv = mesh->uvs ?
                                   std::array<Point2, 3>{mesh->uvs[indices[0]], mesh->uvs[indices[0]], mesh->uvs[indices[0]]} :
                                   std::array<Point2, 3>{Point2{0, 0}, Point2{1, 0}, Point2{1, 1}};
    its.uv = uv[0] * ti->b0 + uv[1] * ti->b1 + uv[2] * ti->b2;

    Vector2 duv02 = uv[0] - uv[2],
            duv12 = uv[1] - uv[2];
    Vector3 dp02 = p0 - p2, dp12 = p1 - p2;
    double Det =
        duv02[0] * duv12[1] - duv02[1] * duv12[0];

    Vector3 dpdu, dpdv;
    bool degenerateUV = std::abs(Det) < 1e-9f;
    if (!degenerateUV) {
        double invDet = 1 / Det;
        dpdu = (dp02 * duv12[1] - dp12 * duv02[1]) * invDet;
        dpdv = (dp12 * duv02[0] - dp02 * duv12[0]) * invDet;
    } else {
        coordinateSystem(geometryNormal, dpdu, dpdv);
    }

    if (mesh->normals || mesh->tangents) {
        if (mesh->normals) {
            its.shFrame.n = Normalized(mesh->normals[indices[0]] * ti->b0 + mesh->normals[indices[1]] * ti->b1 + mesh->normals[indices[2]] * ti->b2);
        }
        if (mesh->tangents) {
            its.shFrame.s = Normalized(mesh->tangents[indices[0]] * ti->b0 + mesh->tangents[indices[1]] * ti->b1 + mesh->tangents[indices[2]] * ti->b2);
        }
        its.shFrame.t = Cross(its.shFrame.n, its.shFrame.t);
    }

    return ShapeIntersection(its, ti->t);
}

double Triangle::Area() {
    std::shared_ptr<TriangleMesh> mesh = allMeshs[meshIndex];
    const int *indices = &(mesh->indices[3 * faceIndex]);
    Point3 p0 = mesh->vertices[indices[0]], p1 = mesh->vertices[indices[1]], p2 = mesh->vertices[indices[2]];
    return .5 * Cross(p0 - p1, p0 - p2).Length();
}

std::optional<ShapeSample> Triangle::Sample(Point2 u) {
    double b0, b1;
    if (u[0] < u[1]) {
        b0 = u[0] / 2;
        b1 = u[1] - b0;
    } else {
        b1 = u[1] / 2;
        b0 = u[0] - b1;
    }
    double b2 = 1. - b0 - b1;

    std::shared_ptr<TriangleMesh> mesh = allMeshs[meshIndex];
    const int *indices = &(mesh->indices[3 * faceIndex]);
    Point3 p0 = mesh->vertices[indices[0]], p1 = mesh->vertices[indices[1]], p2 = mesh->vertices[indices[2]];
    std::array<Point2, 3> uv = mesh->uvs ?
                                   std::array<Point2, 3>{mesh->uvs[indices[0]], mesh->uvs[indices[0]], mesh->uvs[indices[0]]} :
                                   std::array<Point2, 3>{Point2{0, 0}, Point2{1, 0}, Point2{1, 1}};
    Vector3 n = Normalized(Cross(p1 - p0, p2 - p1));
    if (mesh->normals) {
        Vector3 ns = Normalized(mesh->normals[indices[0]] * b0 + mesh->normals[indices[1]] * b1 + mesh->normals[indices[2]] * b2);
        if (Dot(n, ns) < 0.) n = -n;
    }

    return ShapeSample{p0 * b0 + p1 * b1 + p2 * b2, n, 1. / Area(), uv[0] * b0 + uv[1] * b1 + uv[2]*b2};
}

double Triangle::PDF(const Intersection &intr) {
    return 1. / Area();
}

std::optional<TriangleIntersection> Triangle::IntersectTriangle(const Ray &ray, Point3 p0, Point3 p1, Point3 p2) {
    // Translation
    p0 = p0 - Vector3(ray.origin);
    p1 = p1 - Vector3(ray.origin);
    p2 = p2 - Vector3(ray.origin);

    // Permute
    int i2 = 0;
    if (std::abs(ray.direction[i2]) < std::abs(ray.direction[1])) {
        i2 = 1;
    }
    if (std::abs(ray.direction[i2]) < std::abs(ray.direction[2])) {
        i2 = 2;
    }
    int i0 = (i2 + 1) % 3;
    int i1 = (i0 + 1) % 3;

    Vector3 d = ray.direction;
    d = {d[i0], d[i1], d[i2]};
    p0 = {p0[i0], p0[i1], p0[i2]};
    p1 = {p1[i0], p1[i1], p1[i2]};
    p2 = {p2[i0], p2[i1], p2[i2]};

    // Shear
    double Sx = -d.x / d.z;
    double Sy = -d.y / d.z;
    double Sz = 1 / d.z;
    p0.x += Sx * p0.z;
    p0.y += Sy * p0.z;
    p0.z *= Sz;
    p1.x += Sx * p1.z;
    p1.y += Sy * p1.z;
    p1.z *= Sz;
    p2.x += Sx * p2.z;
    p2.y += Sy * p2.z;
    p2.z *= Sz;

    double e0 = p1.x * p2.y - p2.x * p1.y;
    double e1 = p2.x * p0.y - p0.x * p2.y;
    double e2 = p0.x * p1.y - p1.x * p0.y;
    if ((e0 < 0 || e1 < 0 || e2 < 0) && (e0 > 0 || e1 > 0 || e2 > 0)) {
        return std::nullopt;
    }
    double det = e0 + e1 + e2;
    if (det == 0) {
        return std::nullopt;
    }
    double invDet = 1. / det;
    double b0 = e0 * invDet, b1 = e1 * invDet, b2 = e2 * invDet;
    double t = b0 * p0.z + b1 * p1.z + b2 * p2.z;
    if (t < ray.timeMin || t > ray.timeMax) {
        return std::nullopt;
    }
    return TriangleIntersection{b0, b1, b2, t};
}
