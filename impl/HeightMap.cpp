#include "../headers/HeightMap.h"
#include "../headers/Noise.h"
void HeightMap::generateHeightMap() {
    heightMap.resize(width);
    for (int i = 0; i < width; ++i) {
        heightMap[i].resize(height);
    }
    Noise noise(width, height);
    noise.generateNoise();
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            heightMap[i][j] = noise.noiseData[j * width + i] * 2.0f;
        }
    }
}
