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
#include "config.h"
int width = Config::width;
int height = Config::height;
int screenWidth = 1000;
int screenHeight = 1000;
float speed = Config::speed;
// false for color mode 
// true for texture mode
bool textureMode = Config::textureMode;
bool grounded = Config::grounded;


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
bool fullscreen = false;
bool wPressed = false;
bool sPressed = false;
// camera stuff, need to make movement loop
glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);
glm::mat4 proj = glm::perspective(glm::radians(Config::fov), (float)screenWidth / (float)screenHeight, Config::near, Config::far); //FOV, aspect, near, far
void updateState(GLuint shaderProgram, GLuint vao, GLuint vbo, GLuint ebo, std::vector<GLuint> indices, SDL_Window* window, GLuint tex1, GLuint tex2, GLuint tex3, GLuint tex4, bool textureMode, HeightMap& heightMap){
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
            speed *= 50.0f * deltaTime;
        }
        if (sPressed) {
            speed /= 50.0f * deltaTime;
        }
        if (grounded) {
            int x = clamp((int)(cameraPos.x / Config::worldScale), 0, width - 1);
            int z = clamp((int)(cameraPos.z / Config::worldScale), 0, height - 1);
            // need to interpolate otherwise movement is super jumpy
            float targetHeight = (heightMap.heightMap[x][z] * Config::heightmapMultiplier * Config::worldScale) + Config::groundedHeight;
            cameraPos.y = glm::mix(cameraPos.y, targetHeight, 10.0f * deltaTime);
        }

        glm::vec3 target = cameraPos + direction;
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + direction, cameraUp);
        glm::mat4 model = glm::mat4(1.0f);
        // makes model way bigger in terms of the camera
        model = glm::scale(model, glm::vec3(Config::worldScale, Config::worldScale, Config::worldScale));
        
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
        glUniform1f(glGetUniformLocation(shaderProgram, "waterHeight"), Config::waterHeight);
        glUniform1f(glGetUniformLocation(shaderProgram, "beachHeight"), Config::beachHeight);
        glUniform1f(glGetUniformLocation(shaderProgram, "grassHeight"), Config::grassHeight);
        glUniform1f(glGetUniformLocation(shaderProgram, "rockHeight"), Config::rockHeight);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window);
}
int main() {
    SDL_Window* window = initSDL(screenWidth, screenHeight);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    glEnable(GL_DEPTH_TEST);

    GLuint shaderProgram = getShaders();
    
    GLuint tex1, tex2, tex3, tex4;
    initTextures(shaderProgram, tex1, tex2, tex3, tex4);
    glUniform1i(glGetUniformLocation(shaderProgram, "textureMode"), textureMode);

    HeightMap heightMap(width, height);
    heightMap.generateHeightMap();
    std::vector<float> vertices = populateVertices(heightMap);

    std::vector<GLuint> indices = createTriangles(width, height);

    GLuint vao, vbo, ebo;
    
    setupBuffers(vao, vbo, ebo, vertices, indices);
    
    SDL_SetWindowRelativeMouseMode(window, true);

    while(true){
        InputHandler inputHandler(direction, pitch, yaw, &upPressed, &downPressed, &leftPressed, &rightPressed, &textureMode, &grounded, &fullscreen, &wPressed, &sPressed, window);
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(inputHandler.handleInput(event) == -1){
                return 0;
            }
            if (event.type == SDL_EVENT_WINDOW_RESIZED) {
                screenWidth = event.window.data1;
                screenHeight = event.window.data2;
                glViewport(0, 0, screenWidth, screenHeight);
                proj = glm::perspective(glm::radians(Config::fov), (float)screenWidth / (float)screenHeight, Config::near, Config::far);
            }
        }
        updateState(shaderProgram, vao, vbo, ebo, indices, window, tex1, tex2, tex3, tex4, textureMode, heightMap);
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteProgram(shaderProgram);

    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
