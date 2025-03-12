#include "Film.h"

#include "stb_image.h"
#include "stb_image_write.h"
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
    std::vector<unsigned char> image(fullResolution.x * fullResolution.y * 3);
    for (int x = pixelBounding.pMin.x; x < pixelBounding.pMax.x; ++x) {
        for (int y = pixelBounding.pMin.y; y < pixelBounding.pMax.y; ++y) {
            int index = x + y * fullResolution.y;
            double pixelWeight = pixels[index].sumWeight;
            if (pixelWeight > 0) {
                image[index * 3] = int(255 * pixels[index].sumRGB[0] / pixelWeight);
                image[index * 3 + 1] = int(255 * pixels[index].sumRGB[1] / pixelWeight);
                image[index * 3 + 2] = int(255 * pixels[index].sumRGB[2] / pixelWeight);
            }
        }
    }
    stbi_flip_vertically_on_write(1);
    stbi_write_png("output.png", fullResolution.x, fullResolution.y, 3, image.data(), 0);
}
