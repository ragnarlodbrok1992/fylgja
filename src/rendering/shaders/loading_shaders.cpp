#include "loading_shaders.hpp"

#include <SDL3/SDL_filesystem.h>


// Test main
// Paths relative to the binary location 
static const char* vert_shader_path = "../src/assets/shaders/vert_shader.glsl";
static const char* frag_shader_path = "../src/assets/shaders/frag_shader.glsl";

void load_shader_code(char* shader, std::filesystem::path shader_file_path) {
  std::ifstream shader_file(shader_file_path);
  if (!shader_file.is_open()) {
    SDL_Log("Failed to open shader file: %s", shader_file_path.string().c_str());
    return;
  }

  // How big is the file?
  shader_file.seekg(0, std::ios::end);
  size_t file_size = shader_file.tellg();
  shader_file.seekg(0, std::ios::beg);
  SDL_Log("Shader file size: %zu bytes", file_size);

  // Load the shader code into the buffer
  shader_file.read(shader, file_size);
  shader[file_size] = '\0'; // Null-terminate the string]
  shader_file.close();

  SDL_Log("Shader code loaded successfully from: %s", shader_file_path.string().c_str());
}

int main() {
  // Log absolute path to the source code directory at the time of compilation
  // Using __FILE__ to get the current file path
  const char* source_code_dir = __FILE__;
  SDL_Log("Source code directory: %s", source_code_dir);

  // Log path where binary is located
  const char* binary_path = SDL_GetBasePath();
  SDL_Log("Binary path: %s", binary_path);

  std::filesystem::path vert_shader_test_path = std::filesystem::path(binary_path) / vert_shader_path;
  std::filesystem::path frag_shader_test_path = std::filesystem::path(binary_path) / frag_shader_path;

  char* vertexShader = new char[4096]; // Allocate enough space for the shader code
  load_shader_code(vertexShader, std::filesystem::path(vert_shader_test_path));
  SDL_Log("Vertex Shader Code:\n%s", vertexShader);

  SDL_Log("----------------------------------------------------");

  char* fragmentShader = new char[4096]; // Allocate enough space for the shader code
  load_shader_code(fragmentShader, std::filesystem::path(frag_shader_test_path));
  SDL_Log("Fragment Shader Code:\n%s", fragmentShader);

  return 0;
}
