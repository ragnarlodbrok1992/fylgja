#ifndef _LOADING_SHADERS_HPP
#define _LOADING_SHADERS_HPP

#include <filesystem>
#include <fstream>

#include <glad/glad.h>

#include <SDL3/SDL_log.h>
#include <SDL3/SDL_opengl.h>

void load_shader_code(char* shader, std::filesystem::path shader_file_path); 
GLuint compileShader(GLenum type, const char* source);
GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource);


#endif // _LOADING_SHADERS_HPP
