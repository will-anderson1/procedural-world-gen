#ifndef CONFIG_H
#define CONFIG_H

struct Config {
    // ! heightmap settings
    int width = 1000;
    int height = 1000;


    // ! camera settings
    float fov = 100.0f;
    float near = 0.1f;
    float far = 50000.0f;

    // ! visual settings 
    float speed = 2000.0f;
    float groundedSpeed = 1000.0f;
    bool textureMode = true;
    bool grounded = false;
    bool fullscreen = false;


    // ! world generation settings
    // changing this makes broad terrain more varied
    float noise1Frequency = 0.45f;
    // changing this makes small alreas more varied
    float noise2Frequency = 2.25f;
    // threshold for exaggerating mountains
    float mountainThreshold = 0.5f;
    // exaggerates mountains
    float mountainHeightMultiplier = 2.0f;
    // makes the heightmap values larger (or smaller)
    float heightmapMultiplier = 30.0f;
    // height of the camera walking on the ground
    float groundedHeight = 300.0f;
    // makes the world bigger by multiplying the vertices by this value
    float worldScale = 100.0f;

    // ! color and texture settings
    float waterHeight = 0.45f;
    float beachHeight = 0.55f;
    float grassHeight = 0.75f;
    float rockHeight = 0.95f;
    // a lower number makes the texture bigger
    float textureScale = 50.0f;
};

#endif