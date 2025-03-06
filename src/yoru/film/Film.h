#pragma once
#include "base/Vector.h"
#include "base/BoundingBox.h"
#include "color/Spectrum.h"

#include <string>
#include <vector>
struct FilmBaseParameters {
    Point2i fullResolution;
    BBox2i pixelBounding;
    std::string fileName;
};
class Film {
public:
    Film(const FilmBaseParameters &baseParams) : fullResolution(baseParams.fullResolution), pixelBounding(baseParams.pixelBounding), fileName(baseParams.fileName){};

    virtual void AddSample(const Point2i &pFilm, SampledSpectrum spectrum, const SampledWavelengths &lamda, double weight) = 0;
    virtual void WriteImage() = 0;

    Point2i GetFullResolution() const { return fullResolution; }
    BBox2i GetPixelBounding() const { return pixelBounding; }
    std::string GetFileName() const { return fileName; }

protected:
    Point2i fullResolution;
    BBox2i pixelBounding;
    std::string fileName;
};

class RGBFilm : public Film {
public:
    RGBFilm(const FilmBaseParameters &baseParams);

    struct Pixel {
        double sumRGB[3] = {0., 0., 0.};
        double sumWeight = 0.;
    };
    virtual void AddSample(const Point2i &pFilm, SampledSpectrum spectrum, const SampledWavelengths &lamda, double weight) override;
    
    virtual void WriteImage();

protected:
    std::vector<Pixel> pixels;
};