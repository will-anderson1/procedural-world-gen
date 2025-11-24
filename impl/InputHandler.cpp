#include "../headers/InputHandler.h"
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

InputHandler::InputHandler(glm::vec3& direction, float& pitch, float& yaw, bool* upPressed, bool* downPressed, bool* leftPressed, bool* rightPressed, bool* textureMode, bool* grounded, bool* fullscreen, bool* wPressed, bool* sPressed, SDL_Window* window) : direction(direction), pitch(pitch), yaw(yaw), upPressed(upPressed), downPressed(downPressed), leftPressed(leftPressed), rightPressed(rightPressed), textureMode(textureMode), grounded(grounded), fullscreen(fullscreen), wPressed(wPressed), sPressed(sPressed), window(window) {
}

int InputHandler::handleInput(SDL_Event& event) {
    if(event.type == SDL_EVENT_QUIT){
        SDL_Quit();
        return -1;
    }
    if (event.type == SDL_EVENT_MOUSE_MOTION) {

        yaw += event.motion.xrel * 0.1f;
        pitch += -event.motion.yrel * 0.1f;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction = glm::normalize(direction);
    }
    if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.key == SDLK_UP) {
            *upPressed = true;
        }
        if (event.key.key == SDLK_DOWN) {
            *downPressed = true;
        }
        if (event.key.key == SDLK_LEFT) {
            *leftPressed = true;
        }
        if (event.key.key == SDLK_RIGHT) {
            *rightPressed = true;
        }
        if (event.key.key == SDLK_W) {
            *wPressed = true;
        }
        if (event.key.key == SDLK_S) {
            *sPressed = true;
        }
    }
    if (event.type == SDL_EVENT_KEY_UP) {
        if (event.key.key == SDLK_UP) {
            *upPressed = false;
        }
        if (event.key.key == SDLK_DOWN) {
            *downPressed = false;
        }
        if (event.key.key == SDLK_LEFT) {
            *leftPressed = false;
        }
        if (event.key.key == SDLK_RIGHT) {
            *rightPressed = false;
        }
        if (event.key.key == SDLK_C) {
            *textureMode = !*textureMode;
        }
        if (event.key.key == SDLK_M) {
            *grounded = !*grounded;
        }
        if (event.type == SDL_EVENT_KEY_UP && event.key.key == SDLK_F){ //If "f" is pressed
            *fullscreen = !*fullscreen;
            SDL_SetWindowFullscreen(window, *fullscreen);
        }
        if (event.key.key == SDLK_ESCAPE){
            *fullscreen = false;
            SDL_SetWindowFullscreen(window, *fullscreen);
        }
        if (event.key.key == SDLK_Q){
            SDL_Quit();
            exit(0);
        }
        if (event.key.key == SDLK_W) {
            *wPressed = false;
        }
        if (event.key.key == SDLK_S) {
            *sPressed = false;
        }
    }
    return 0;
}