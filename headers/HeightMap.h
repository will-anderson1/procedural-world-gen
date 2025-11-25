#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <vector>
#include "Config.h"
struct HeightMap {
    std::vector<std::vector<float> > heightMap;
    float minHeight;
    float maxHeight;
    int width;
    int height;
    HeightMap(int width, int height) : width(width), height(height) {}
    void generateHeightMap(const Config& config);
};

#endif
