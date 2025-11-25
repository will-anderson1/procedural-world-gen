#include "../headers/Noise.h"
#include "../headers/FastNoiseLite.h"
#include "../headers/config.h"

Noise::Noise(int width, int height) : width(width), height(height) {
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise2.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noiseData.resize(width * height);
}

void Noise::generateNoise(const Config& config) {
    int index = 0;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // changing these constants make broad terrain more varied
            float n1 = noise.GetNoise((float)x * config.noise1Frequency, (float)y * config.noise1Frequency);
            // changing these constatns make small alreas more varied
            float n2 = noise2.GetNoise((float)x * config.noise2Frequency, (float)y * config.noise2Frequency);
            float combined = n1 + (n2 * 0.2f);
            // makes mountains taller
            if (combined > config.mountainThreshold) {
                combined = pow(combined, 2.0f) * config.mountainHeightMultiplier;
            }
            
            noiseData[index++] = combined;
        }
    }
}
