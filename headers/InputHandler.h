#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H
#include <glm/glm.hpp>
#include <SDL3/SDL.h>
struct InputHandler {
    glm::vec3& direction;
    float& pitch;
    float& yaw;
    bool* upPressed;
    bool* downPressed;
    bool* leftPressed;
    bool* rightPressed;
    bool* textureMode;
    bool* grounded;
    bool* fullscreen;
    bool* wPressed;
    bool* sPressed;
    SDL_Window* window;
    InputHandler(glm::vec3& direction, float& pitch, float& yaw, bool* upPressed, bool* downPressed, bool* leftPressed, bool* rightPressed, bool* textureMode, bool* grounded, bool* fullscreen, bool* wPressed, bool* sPressed, SDL_Window* window);
    int handleInput(SDL_Event& event);
};
#endif