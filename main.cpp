#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "headers/glad/glad.h"
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "headers/HeightMap.h"
#include "headers/utils.h"
#include "headers/InputHandler.h"
#include "headers/ConfigHelper.h"
#include "headers/Config.h"
#include <thread>
#include <chrono>
Config config;
bool configUpdated = false;
int width;
int height;
int screenWidth;
int screenHeight;
float speed;
// false for color mode 
// true for texture mode
bool textureMode;
bool grounded;
bool fullscreen;


glm::vec3 cameraPos = glm::vec3(0.0f, 100.0f, 200.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float pitch = 0.0f;
float yaw = 0.0f;
float lastTime = 0.0f;
bool upPressed = false;
bool downPressed = false;
bool leftPressed = false;
bool rightPressed = false;
bool wPressed = false;
bool sPressed = false;

GLuint shaderProgram;
GLuint tex1, tex2, tex3, tex4;
SDL_Window* window;

// camera stuff, need to make movement loop
glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);
glm::mat4 proj = glm::mat4(1.0f);


void updateState(GLuint shaderProgram, GLuint vao, GLuint vbo, GLuint ebo, const std::vector<GLuint>& indices, SDL_Window* window, GLuint tex1, GLuint tex2, GLuint tex3, GLuint tex4, bool textureMode, HeightMap& heightMap){
    float currentTime = SDL_GetTicks() / 1000.0f;
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        if (upPressed) {
            cameraPos += direction * speed * deltaTime;
        }
        if (downPressed) {
            cameraPos -= direction * speed * deltaTime;
        }
        if (leftPressed) {
            cameraPos -= glm::cross(direction, cameraUp) * speed * deltaTime;
        }
        if (rightPressed) {
            cameraPos += glm::cross(direction, cameraUp) * speed * deltaTime;
        }
        if (wPressed) {
            // speed += 50.0f * deltaTime;
            speed *= 1.001f;
        }
        if (sPressed) {
            // speed /= 50.0f * deltaTime;
            speed *= 0.999f;
        }
        if (grounded) {
            int x = clamp((int)(cameraPos.x / config.worldScale), 0, width - 1);
            int z = clamp((int)(cameraPos.z / config.worldScale), 0, height - 1);
            // need to interpolate otherwise movement is super jumpy
            float targetHeight = (heightMap.heightMap[x][z] * config.heightmapMultiplier * config.worldScale) + config.groundedHeight;
            cameraPos.y = glm::mix(cameraPos.y, targetHeight, 10.0f * deltaTime);
        }

        glm::vec3 target = cameraPos + direction;
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + direction, cameraUp);
        glm::mat4 model = glm::mat4(1.0f);
        // makes model way bigger in terms of the camera
        model = glm::scale(model, glm::vec3(config.worldScale, config.worldScale, config.worldScale));
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);
        
        GLuint uniModel = glGetUniformLocation(shaderProgram, "model");
        GLuint uniView = glGetUniformLocation(shaderProgram, "view");
        GLuint uniProj = glGetUniformLocation(shaderProgram, "proj");
        
        glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tex4);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, tex2);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, tex3);
        glUniform1i(glGetUniformLocation(shaderProgram, "textureMode"), textureMode);
        glUniform1f(glGetUniformLocation(shaderProgram, "waterHeight"), config.waterHeight);
        glUniform1f(glGetUniformLocation(shaderProgram, "beachHeight"), config.beachHeight);
        glUniform1f(glGetUniformLocation(shaderProgram, "grassHeight"), config.grassHeight);
        glUniform1f(glGetUniformLocation(shaderProgram, "rockHeight"), config.rockHeight);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window);
}
void startShaderProgram(){
    proj = glm::perspective(glm::radians(config.fov), (float)screenWidth / (float)screenHeight, config.near, config.far);
    HeightMap heightMap(width, height);
    heightMap.generateHeightMap(config);
    std::vector<float> vertices = populateVertices(heightMap, config);

    std::vector<GLuint> indices = createTriangles(width, height);

    GLuint vao, vbo, ebo;
    
    setupBuffers(vao, vbo, ebo, vertices, indices);
    
    SDL_SetWindowRelativeMouseMode(window, true);

    InputHandler inputHandler(direction, pitch, yaw, &upPressed, &downPressed, &leftPressed, &rightPressed, &textureMode, &grounded, &fullscreen, &wPressed, &sPressed, window);
    while(true){
        
        if (configUpdated) {
            configUpdated = false;
            width = config.width;
            height = config.height;
            speed = config.speed;
            textureMode = config.textureMode;
            grounded = config.grounded;
            proj = glm::perspective(glm::radians(config.fov), (float)screenWidth / (float)screenHeight, config.near, config.far);
            
            heightMap.width = width;
            heightMap.height = height;
            heightMap.generateHeightMap(config);
            vertices = populateVertices(heightMap, config);
            indices = createTriangles(width, height);
            
            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(1, &vbo);
            glDeleteBuffers(1, &ebo);
            
            setupBuffers(vao, vbo, ebo, vertices, indices);
        }
        glUniform1f(glGetUniformLocation(shaderProgram, "textureScale"), config.textureScale);

        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(inputHandler.handleInput(event) == -1){
                exit(0);
            }
            if (event.type == SDL_EVENT_WINDOW_RESIZED) {
                screenWidth = event.window.data1;
                screenHeight = event.window.data2;
                glViewport(0, 0, screenWidth, screenHeight);
                proj = glm::perspective(glm::radians(config.fov), (float)screenWidth / (float)screenHeight, config.near, config.far);
            }
        }
        updateState(shaderProgram, vao, vbo, ebo, indices, window, tex1, tex2, tex3, tex4, textureMode, heightMap);
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}
void configUpdaterThread() {
    while (true) {
        configUpdated = ConfigHelper::loadConfig("config.json", config);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
int main() {
    ConfigHelper::loadConfig("config.json", config);
    width = config.width;
    height = config.height;
    screenWidth = 1000;
    screenHeight = 1000;
    speed = config.speed;
    textureMode = config.textureMode;
    grounded = config.grounded;
    fullscreen = config.fullscreen;
    std::thread t(configUpdaterThread);
    
    window = initSDL(screenWidth, screenHeight);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    SDL_SetWindowRelativeMouseMode(window, true);
    glEnable(GL_DEPTH_TEST);

    shaderProgram = getShaders();
    
    initTextures(shaderProgram, tex1, tex2, tex3, tex4);
    glUniform1i(glGetUniformLocation(shaderProgram, "textureMode"), textureMode);
    startShaderProgram();

    glDeleteProgram(shaderProgram);

    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    t.detach();
    return 0;
}
