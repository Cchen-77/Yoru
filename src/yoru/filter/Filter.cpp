#include "Filter.h"

FilterSampler::FilterSampler(std::shared_ptr<Filter> filter) : filter(filter) {
    BBox2 domain(Point2(-filter->Radius()), Point2(filter->Radius()));
    table.resize(32);
    for (int i = 0; i < 32; ++i) {
        table[i].resize(32);
        for (int j = 0; j < 32; ++j) {
            table[i][j] = filter->Evaluate(domain.Lerp({(i + .5) / 32, (j + .5) / 32}));
        }
    }
    distrib = PiecewiseConstant2D(table, domain);
}

FilterSample FilterSampler::Sample(Point2 u) const{
    double pdf;
    Point2i offset;
    Point2 p = distrib.Sample(u, &pdf, &offset);
    FilterSample fs;
    fs.p = p;
    fs.weight = table[offset[0]][offset[1]] / pdf;
    return fs;
}
