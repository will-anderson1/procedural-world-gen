#ifndef NOISE_H
#define NOISE_H

#include <vector>
#include "FastNoiseLite.h"
#include "config.h"
struct Noise {
    int width;
    int height;
    FastNoiseLite noise;
    FastNoiseLite noise2;
    std::vector<float> noiseData;
    Noise(int width, int height);
    void generateNoise(const Config& config);
};
#endif