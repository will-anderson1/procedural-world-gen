struct Config {

    // heightmap settings
    static constexpr int width = 1000;
    static constexpr int height = 1000;


    // camera settings
    static constexpr float fov = 100.0f;
    static constexpr float near = 0.1f;
    static constexpr float far = 50000.0f;

    // visual settings 
    static constexpr float speed = 2000.0f;
    static constexpr float groundedSpeed = 1000.0f;
    static constexpr bool textureMode = true;
    static constexpr bool grounded = false;
    static constexpr bool fullscreen = false;


    // world generation settings
    static constexpr float noise1Frequency = 0.45f;
    static constexpr float noise2Frequency = 2.25f;
    static constexpr float mountainThreshold = 0.5f;
    static constexpr float mountainHeightMultiplier = 2.0f;
    static constexpr float heightmapMultiplier = 30.0f;
    static constexpr float groundedHeight = 300.0f;
    static constexpr float worldScale = 100.0f;

    // color and texture settings
    static constexpr float waterHeight = 0.45f;
    static constexpr float beachHeight = 0.55f;
    static constexpr float grassHeight = 0.75f;
    static constexpr float rockHeight = 0.95f;
};
