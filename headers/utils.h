#ifndef UTILS_H
#define UTILS_H

#include "HeightMap.h"
#include "glad/glad.h"
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Config.h"
void write_ppm(const char* imgName, int width, int height, int bits, const HeightMap& heightMap);
GLuint getShaders();
SDL_Window* initSDL(int screenWidth, int screenHeight);
void setupBuffers(GLuint& vao, GLuint& vbo, GLuint& ebo, std::vector<float>& vertices, std::vector<unsigned int>& indices);
void initTextures(GLuint shaderProgram, GLuint& texture1, GLuint& texture2, GLuint& texture3, GLuint& texture4);
std::vector<float> populateVertices(const HeightMap& heightMap, const Config& config);
std::vector<GLuint> createTriangles(GLuint width, GLuint height);
int clamp(int a, int lo, int hi);
#endif

