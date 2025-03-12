#include "Sphere.h"

Sphere::Sphere(const Transform &rfo, double radius)
    : Shape(rfo), radius(radius) {
}
BBox3 Sphere::BBox() const {
    return renderFromObject(BBox3({-radius, -radius, -radius}, {radius, radius, radius}));
}

std::optional<ShapeIntersection> Sphere::Intersect(const Ray &ray) const {
    Ray r = (renderFromObject.Inverse())(ray);
    double a = r.direction.x * r.direction.x + r.direction.y * r.direction.y + r.direction.z * r.direction.z;
    double b = 2 * (r.direction.x * r.origin.x + r.direction.y * r.origin.y + r.direction.z * r.origin.z);
    double c = r.origin.x * r.origin.x + r.origin.y * r.origin.y + r.origin.z * r.origin.z - radius * radius;

    double delta = b * b - 4 * a * c;
    if (delta < 0) {
        return std::nullopt;
    }
    double rootDelta = std::sqrt(delta);
    double t0, t1;
    double q;
    if (b < 0)
        q = .5 * (-b - rootDelta);
    else
        q = .5 * (b + rootDelta);
    t0 = q / a;
    t1 = c / q;
    if (t0 > t1) {
        std::swap(t0, t1);
    }
    double tHit = t0;
    if (tHit < r.timeMin || tHit > r.timeMin) {
        tHit = t1;
    }
    if (tHit < r.timeMin || tHit > r.timeMax) {
        return std::nullopt;
    }
    Intersection its;
    Point3 pHit = r(tHit);
    its.wo = -ray.direction;
    its.p = renderFromObject(pHit);

    Vector3 normal = Normalized(Vector3(pHit));
    its.geoFrame.n = Normalized(renderFromObject(normal));

    double phi = std::atan2(normal.y, normal.x);
    if (phi < 0) phi += 2 * M_PI;
    its.uv = {.5 * phi / M_PI,
              std::acos(normal.z) / M_PI};

    its.dpdu = renderFromObject(Vector3(-normal.y, normal.x, 0.) * radius * 2 * M_PI);

    double sinTheta = std::sqrt(normal.x * normal.x + normal.y * normal.y);
    if (sinTheta > 0.) {
        double invSinTheta = 1. / sinTheta;
        double cosPhi = normal.x * invSinTheta, sinPhi = normal.y * invSinTheta;
        its.dpdv = renderFromObject(Vector3(normal.z * cosPhi, normal.z * cosPhi, -sinTheta) * radius * M_PI);

        its.geoFrame.s = Normalized(its.dpdu);
        its.geoFrame.t = Normalized(its.dpdv);

    } else {

        const double cosPhi = 0, sinPhi = 1;
        its.dpdv = renderFromObject(Vector3(cosPhi, sinPhi, 0.) * radius * M_PI);
        coordinateSystem(its.geoFrame.n, its.geoFrame.s, its.geoFrame.t);
    }

    its.shFrame = its.geoFrame;

    return ShapeIntersection(its, tHit);
}

double Sphere::Area() const {
    return 4 * M_PI * radius * radius;
}

std::optional<ShapeSample> Sphere::Sample(Point2 u) const {
    double phi = u[0] * 2 * M_PI;
    double cosTheta = 1 - 2 * u[1];
    double sinTheta = std::sqrt(1 - cosTheta * cosTheta);
    Vector3 normal = {std::cos(phi) * sinTheta,
                      std::sin(phi) * sinTheta,
                      cosTheta};
    Point3 point = Point3(normal) * radius;

    normal = Normalized(normal);
    Point2 uv = {u[0], std::acos(cosTheta) / M_PI};

    return ShapeSample{renderFromObject(point), Normalized(renderFromObject(normal)), 1 / Area(), uv};
}

double Sphere::PDF(const Intersection &intr) const {
    return 1. / Area();
}

// TODO
std::optional<ShapeSample> Sphere::Sample(const ShapeSampleContext &ctx, Point2 u) const {
    Point3 p = ctx.p;
    Point3 center = renderFromObject(Point3(0, 0, 0));
    if ((p - center).Length2() <= radius * radius) {
        auto ss = Sample(u);
        if (!ss) {
            return std::nullopt;
        }
        Vector3 d = p - ss->p;
        ss->pdf /= AbsDot(ss->geoNormal, Normalized(d));
        ss->pdf *= d.Length2();
        return ss;
    } else {
        double sinThetaMax = radius / (p - center).Length();
        double sin2ThetaMax = sinThetaMax * sinThetaMax;
        double cosThetaMax = std::sqrt(1 - sin2ThetaMax);

        double cosTheta = (cosThetaMax - 1.) * u[0] + 1.;
        double sin2Theta = 1. - cosTheta * cosTheta;
        double phi = 2 * M_PI * u[1];

        double cosAlpha = sin2Theta / sinThetaMax + cosTheta * std::sqrt(1 - sin2Theta / sin2ThetaMax);
        double sinAlpha = std::sqrt(1 - cosAlpha * cosAlpha);
        Frame frame(Normalized((p - center)));

        Vector3 local = {sinAlpha * std::cos(phi), sinAlpha * std::sin(phi), cosAlpha};
        Vector3 normal = Normalized(frame.FromLocal(local));

        ShapeSample ss;
        ss.geoNormal = normal;
        ss.p = center + ss.geoNormal * radius;
        ss.pdf = 2 * M_PI * (1 - cosThetaMax);

        {
            Vector3 objNormal = (renderFromObject.Inverse())(ss.geoNormal);
            double phi = std::atan2(normal.y, normal.z);
            if (phi < 0) phi += 2 * M_PI;
            ss.uv = {.5 * phi * M_INVPI,
                     std::acos(normal.z) / M_PI};
        }

        return ss;
    }
}

double Sphere::PDF(const ShapeSampleContext &ctxt, const Intersection &its) const {
    Point3 p = ctxt.p;
    Point3 center = renderFromObject(Point3(0, 0, 0));
    if ((p - center).Length2() <= radius * radius) {
        Vector3 d = p - its.p;
        double pdf = 1. / Area() * d.Length2() / AbsDot(its.geoFrame.n, Normalized(d));
        if (std::isinf(pdf)) {
            pdf = 0;
        }
        return pdf;
    } else {
        double sinThetaMax = radius / (p - center).Length();
        double sin2ThetaMax = sinThetaMax * sinThetaMax;
        double cosThetaMax = std::sqrt(1 - sin2ThetaMax);

        return 1. / (2 * M_PI * (1 - cosThetaMax));
    }
}
