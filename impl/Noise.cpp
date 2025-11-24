#include "../headers/Noise.h"
#include "../headers/FastNoiseLite.h"
#include "../config.h"
Noise::Noise(int width, int height) : width(width), height(height) {
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise2.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noiseData.resize(width * height);
}

void Noise::generateNoise() {
    int index = 0;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // changing these constants make broad terrain more varied
            float n1 = noise.GetNoise((float)x * Config::noise1Frequency, (float)y * Config::noise1Frequency);
            // changing these constatns make small alreas more varied
            float n2 = noise2.GetNoise((float)x * Config::noise2Frequency, (float)y * Config::noise2Frequency);
            float combined = n1 + (n2 * 0.2f);
            // makes mountains taller
            if (combined > Config::mountainThreshold) {
                combined = pow(combined, 2.0f) * Config::mountainHeightMultiplier;
            }
            
            noiseData[index++] = combined;
        }
    }
}
