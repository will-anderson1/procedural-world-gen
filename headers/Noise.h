#ifndef NOISE_H
#define NOISE_H

#include <vector>
#include "FastNoiseLite.h"
struct Noise {
    int width;
    int height;
    FastNoiseLite noise;
    FastNoiseLite noise2;
    std::vector<float> noiseData;
    Noise(int width, int height);
    void generateNoise();
};
#endif