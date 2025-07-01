#ifndef _LOADING_SHADERS_HPP
#define _LOADING_SHADERS_HPP

#include <filesystem>
#include <fstream>

#include <SDL3/SDL_log.h>

void load_shader_code(char* shader, std::filesystem::path shader_file_path); 


#endif // _LOADING_SHADERS_HPP
