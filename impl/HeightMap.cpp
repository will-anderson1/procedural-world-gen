#include "../headers/HeightMap.h"
#include "../headers/Noise.h"
#include "../headers/config.h"
void HeightMap::generateHeightMap(const Config& config) {
    heightMap.resize(width);
    for (int i = 0; i < width; ++i) {
        heightMap[i].resize(height);
    }
    Noise noise(width, height);
    noise.generateNoise(config);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            heightMap[i][j] = noise.noiseData[j * width + i] * 2.0f;
        }
    }
}
