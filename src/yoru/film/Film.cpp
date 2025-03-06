#include "Film.h"

RGBFilm::RGBFilm(const FilmBaseParameters &baseParams) : Film(baseParams) {
    pixels.resize(fullResolution.x * fullResolution.y);
}

void RGBFilm::AddSample(const Point2i &pFilm, SampledSpectrum spectrum, const SampledWavelengths &lamda, double weight) {
    int index = pFilm.x + pFilm.y * fullResolution.x;
    // TODO: real sampledSpecturm to RGB with sensor
    // now a temp implementation since we do not really have SampledSpectrum
    for (int i = 0; i < 3; ++i) {
        pixels[index].sumRGB[i] += spectrum[i];
    }
    pixels[index].sumWeight += weight;
}

void RGBFilm::WriteImage() {

}
