#include "Sampling.h"

#include<algorithm>
PiecewiseConstant1D::PiecewiseConstant1D(const std::vector<double>& _func, double min, double max) : func(_func.begin(), _func.end()), min(min), max(max) {
    int n = func.size();
    for (int i = 0; i < n; ++i) {
        func[i] = std::abs(func[i]);
    }
    cdf.resize(n);
    cdf[0] = func[0] * (max - min) / n;
    for (int i = 1; i < n; ++i) {
        cdf[i] = cdf[i - 1] + func[i] * (max - min) / n;
    }
    intFunc = cdf[n - 1];
    if (intFunc == 0) {
        for (int i = 0; i < n; ++i) {
            cdf[i] = (i + 1.) / n;
        }
    } else {
        for (int i = 0; i < n; ++i) {
            cdf[i] /= intFunc;
        }
    }
}

double PiecewiseConstant1D::Sample(double u, double *pdf, int* offset) const {
    int n = func.size();
    int index = std::upper_bound(cdf.begin(), cdf.end(), u) - cdf.begin();
    if (offset) {
        *offset = index;
    }
    if (pdf) {
        *pdf = func[index] / intFunc;
    }

    double du = u;
    if (index > 0) {
        du -= cdf[index - 1];
        du /= cdf[index] - cdf[index - 1];
    } else {
        du /= cdf[0];
    }
    du += index;
    return du * (max - min) / n;
}

double PiecewiseConstant1D::Integral() const {
    return intFunc;
}

PiecewiseConstant2D::PiecewiseConstant2D(const std::vector<std::vector<double>> &table, BBox2 bbox) {
    nu = table.size();
    std::vector<double> funcU;
    for (int i = 0; i < nu; ++i) {
        pUConditionedV.emplace_back(table[i], bbox.pMin.y, bbox.pMax.y);
        funcU.push_back(pUConditionedV[i].Integral());
    }
    pU = PiecewiseConstant1D(funcU, bbox.pMin.x, bbox.pMax.x);
}

Point2 PiecewiseConstant2D::Sample(Point2 u, double *pdf, Point2i *offset) const {
    Point2 uv;
    Point2i uvi;
    Point2 uvpdf;
    uv[0] = pU.Sample(u[0], &uvpdf[0], &uvi[0]);
    uv[1] =pUConditionedV[uvi[0]].Sample(u[1], &uvpdf[1], &uvi[1]);
    if (offset) {
        *offset = uvi;
    }
    if (pdf) {
        *pdf = uvpdf[0] * uvpdf[1];
    }
    return uv;
}

double PiecewiseConstant2D::Integral() {
    return pU.Integral();
}
