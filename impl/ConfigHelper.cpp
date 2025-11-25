#include "../headers/ConfigHelper.h"
#include <fstream>
#include <string>
#include "../headers/json.hpp"
#include "../headers/config.h"
#include <iostream>
bool floatsEqual(float a, float b) {
    return abs(a - b) < 0.01f;
}
bool ConfigHelper::loadConfig(const std::string& filename, Config& config) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    nlohmann::json j;
    file >> j;

    bool changed = false;

    if(config.width != j.value("width", config.width)) changed = true;
    if(config.height != j.value("height", config.height)) changed = true;
    if(!floatsEqual(config.fov, j.value("fov", config.fov))) changed = true;
    if(!floatsEqual(config.near, j.value("near", config.near))) changed = true;
    if(!floatsEqual(config.far, j.value("far", config.far))) changed = true;
    if(!floatsEqual(config.speed, j.value("speed", config.speed))) changed = true;
    if(!floatsEqual(config.groundedSpeed, j.value("groundedSpeed", config.groundedSpeed))) changed = true;
    if(config.textureMode != j.value("textureMode", config.textureMode)) changed = true;
    if(config.grounded != j.value("grounded", config.grounded)) changed = true;
    if(config.fullscreen != j.value("fullscreen", config.fullscreen)) changed = true;
    
    if(!floatsEqual(config.noise1Frequency, j.value("noise1Frequency", config.noise1Frequency))) changed = true;
    if(!floatsEqual(config.noise2Frequency, j.value("noise2Frequency", config.noise2Frequency))) changed = true;
    if(!floatsEqual(config.mountainThreshold, j.value("mountainThreshold", config.mountainThreshold))) changed = true;
    if(!floatsEqual(config.mountainHeightMultiplier, j.value("mountainHeightMultiplier", config.mountainHeightMultiplier))) changed = true;
    if(!floatsEqual(config.heightmapMultiplier, j.value("heightmapMultiplier", config.heightmapMultiplier))) changed = true;
    if(!floatsEqual(config.groundedHeight, j.value("groundedHeight", config.groundedHeight))) changed = true;
    if(!floatsEqual(config.worldScale, j.value("worldScale", config.worldScale))) changed = true;
    
    if(!floatsEqual(config.waterHeight, j.value("waterHeight", config.waterHeight))) changed = true;
    if(!floatsEqual(config.beachHeight, j.value("beachHeight", config.beachHeight))) changed = true;
    if(!floatsEqual(config.grassHeight, j.value("grassHeight", config.grassHeight))) changed = true;
    if(!floatsEqual(config.rockHeight, j.value("rockHeight", config.rockHeight))) changed = true;
    if(!floatsEqual(config.textureScale, j.value("textureScale", config.textureScale))) changed = true;

    if (changed) {
        config.width = j.value("width", config.width);
        config.height = j.value("height", config.height);
        config.fov = j.value("fov", config.fov);
        config.near = j.value("near", config.near);
        config.far = j.value("far", config.far);
        config.speed = j.value("speed", config.speed);
        config.groundedSpeed = j.value("groundedSpeed", config.groundedSpeed);
        config.textureMode = j.value("textureMode", config.textureMode);
        config.grounded = j.value("grounded", config.grounded);
        config.fullscreen = j.value("fullscreen", config.fullscreen);
        
        config.noise1Frequency = j.value("noise1Frequency", config.noise1Frequency);
        config.noise2Frequency = j.value("noise2Frequency", config.noise2Frequency);
        config.mountainThreshold = j.value("mountainThreshold", config.mountainThreshold);
        config.mountainHeightMultiplier = j.value("mountainHeightMultiplier", config.mountainHeightMultiplier);
        config.heightmapMultiplier = j.value("heightmapMultiplier", config.heightmapMultiplier);
        config.groundedHeight = j.value("groundedHeight", config.groundedHeight);
        config.worldScale = j.value("worldScale", config.worldScale);
        
        config.waterHeight = j.value("waterHeight", config.waterHeight);
        config.beachHeight = j.value("beachHeight", config.beachHeight);
        config.grassHeight = j.value("grassHeight", config.grassHeight);
        config.rockHeight = j.value("rockHeight", config.rockHeight);
        config.textureScale = j.value("textureScale", config.textureScale);
    }

    return changed;
}