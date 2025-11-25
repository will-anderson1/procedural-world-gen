#include <fstream>
#include <string>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include "../headers/HeightMap.h"
#include "../headers/glad/glad.h"
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "../headers/Config.h"

int clamp(int a, int lo, int hi){
   if (a < lo){
      return lo;
   }
   if (a > hi){
      return hi;
   }
   return a;
}
void setupBuffers(GLuint& vao, GLuint& vbo, GLuint& ebo, std::vector<float>& vertices, std::vector<unsigned int>& indices){
   glGenVertexArrays(1, &vao);
   glGenBuffers(1, &vbo);
   glGenBuffers(1, &ebo);

   glBindVertexArray(vao);

   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);

   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);
}
void initTextures(GLuint shaderProgram, GLuint& texture1, GLuint& texture2, GLuint& texture3, GLuint& texture4){
   SDL_Surface* surface = SDL_LoadBMP("textures/oceans.bmp");
   glGenTextures(1, &texture1);
   glBindTexture(GL_TEXTURE_2D, texture1);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_BGR, GL_UNSIGNED_BYTE, surface->pixels);
   glGenerateMipmap(GL_TEXTURE_2D);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   SDL_DestroySurface(surface);

   SDL_Surface* surface2 = SDL_LoadBMP("textures/forest.bmp");
   glGenTextures(1, &texture2);
   glBindTexture(GL_TEXTURE_2D, texture2);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface2->w, surface2->h, 0, GL_BGR, GL_UNSIGNED_BYTE, surface2->pixels);
   glGenerateMipmap(GL_TEXTURE_2D);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   SDL_DestroySurface(surface2);

   SDL_Surface* surface3 = SDL_LoadBMP("textures/mountains.bmp");
   glGenTextures(1, &texture3);
   glBindTexture(GL_TEXTURE_2D, texture3);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface3->w, surface3->h, 0, GL_BGR, GL_UNSIGNED_BYTE, surface3->pixels);
   glGenerateMipmap(GL_TEXTURE_2D);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   SDL_DestroySurface(surface3);

   SDL_Surface* surface4 = SDL_LoadBMP("textures/beach.bmp");
   glGenTextures(1, &texture4);
   glBindTexture(GL_TEXTURE_2D, texture4);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface4->w, surface4->h, 0, GL_BGR, GL_UNSIGNED_BYTE, surface4->pixels);
   glGenerateMipmap(GL_TEXTURE_2D);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   SDL_DestroySurface(surface4);

   glUseProgram(shaderProgram);
   glUniform1i(glGetUniformLocation(shaderProgram, "oceanTex"), 0);
   glUniform1i(glGetUniformLocation(shaderProgram, "beachTex"), 1);
   glUniform1i(glGetUniformLocation(shaderProgram, "forestTex"), 2);
   glUniform1i(glGetUniformLocation(shaderProgram, "mountainTex"), 3);
}
void write_ppm(const char* imgName, int width, int height, int bits, const HeightMap& heightMap){
   //Open the texture image file
   std::ofstream ppmFile;
   ppmFile.open(imgName);
   if (!ppmFile){
      printf("ERROR: Could not create file '%s'\n",imgName);
      exit(1);
   }

   //Set this as an ASCII PPM (first line is P3)
   std::string PPM_style = "P3\n";
   ppmFile << PPM_style; //Read the first line of the header    

   //Write out the texture width and height
   ppmFile << width << " "  << height << "\n" ;

   //Set's the 3rd line to 255 (ie., assumes this is an 8 bit/pixel PPM)
   //TODO - HW2: Set the maximum values based on the value of the variable 'bits'
   int maximum = 255;
   ppmFile << maximum << "\n" ;

   int r, g, b;
   for (int i = 0; i < heightMap.height; i++){
      for (int j = 0; j < heightMap.width; j++){
         float val = heightMap.heightMap[j][i];
      //    noise is -1,1
         float normalized = (val + 1.0f) * 0.5f * 255.0f;

         r = clamp((int) round(normalized), 0, maximum);  //Red
         g = clamp((int) round(normalized), 0, maximum);  //Green
         b = clamp((int) round(normalized), 0, maximum);  //Blue
         
         ppmFile << r << " " << g << " "  << b << " " ;
      }
   }
   ppmFile.close();
 }
 GLuint getShaders() {
   std::string vertexCode;
   std::string fragmentCode;
   std::ifstream vShaderFile;
   std::ifstream fShaderFile;
   std::stringstream vShaderStream;
   std::stringstream fShaderStream;

   vShaderFile.open("shaders/vertex_shader.glsl");
   vShaderStream << vShaderFile.rdbuf();
   vShaderFile.close();
   vertexCode = vShaderStream.str();

   fShaderFile.open("shaders/color_shader.glsl");
   fShaderStream << fShaderFile.rdbuf();
   fShaderFile.close();
   fragmentCode = fShaderStream.str();

   const char* vShaderCode = vertexCode.c_str();
   const char * fShaderCode = fragmentCode.c_str();

   GLuint vertex, fragment;
   
   vertex = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vertex, 1, &vShaderCode, NULL);
   glCompileShader(vertex);
   

   fragment = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragment, 1, &fShaderCode, NULL);
   glCompileShader(fragment);
   
   GLuint shader = glCreateProgram();
   glAttachShader(shader, vertex);
   glAttachShader(shader, fragment);
   glLinkProgram(shader);
   
   glDeleteShader(vertex);
   glDeleteShader(fragment);
   return shader;
}

SDL_Window* initSDL(int screenWidth, int screenHeight) {
   // from p4 example
   SDL_Init(SDL_INIT_VIDEO);  //Initialize Graphics (for OpenGL)

   //Print the version of SDL we are using (should be 3.x or higher)
   const int sdl_linked = SDL_GetVersion();
   printf("\nCompiled against SDL version %d.%d.%d ...\n", SDL_VERSIONNUM_MAJOR(SDL_VERSION), SDL_VERSIONNUM_MINOR(SDL_VERSION), SDL_VERSIONNUM_MICRO(SDL_VERSION));
   printf("Linking against SDL version %d.%d.%d.\n", SDL_VERSIONNUM_MAJOR(sdl_linked), SDL_VERSIONNUM_MINOR(sdl_linked), SDL_VERSIONNUM_MICRO(sdl_linked));

  //Ask SDL to get a recent version of OpenGL (3.2 or greater)
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
   
   // Create window
  SDL_Window* window = SDL_CreateWindow("OpenGL Heightmap", screenWidth, screenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
   if (!window) {
       printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
       SDL_Quit();
       exit(1);
   }
   return window;
}
std::vector<float> populateVertices(const HeightMap& heightMap, const Config& config){
   std::vector<float> vertices;
   for(int i = 0; i < heightMap.height; i++){
      for(int j = 0; j < heightMap.width; j++){
         vertices.push_back((float)j);
         vertices.push_back(heightMap.heightMap[j][i] * config.heightmapMultiplier);
         vertices.push_back((float)i);
         // texture coordinates
         vertices.push_back((float)j);
         vertices.push_back((float)i);
      }
   }
   return vertices;
}
std::vector<GLuint> createTriangles(GLuint width, GLuint height){
   std::vector<GLuint> indices;
    for(int i = 0; i < height - 1; i++) {
        for(int j = 0; j < width - 1; j++) {
            int tl = i * width + j;
            int tr = tl + 1;
            int bl = (i + 1) * width + j;
            int br = bl + 1;

            indices.push_back(tl);
            indices.push_back(bl);
            indices.push_back(tr);

            indices.push_back(tr);
            indices.push_back(bl);
            indices.push_back(br);
        }
    }
    return indices;
}