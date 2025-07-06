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

GLuint compileShader(GLenum type, const char* source) {
  GLuint shader = glCreateShader(type);

  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    SDL_Log("Shader compilation failed: %s", infoLog);
  }

  return shader;
}

GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource) {
  GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
  GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  GLint success;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    SDL_Log("Shader program linking failed: %s", infoLog);
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}

GLuint initializeShaderProgram(std::filesystem::path vertex_shader_path, std::filesystem::path fragment_shader_path) {
    char vertex_shader_code[4096]; // Adjust size as needed
    char fragment_shader_code[4096]; // Adjust size as needed

    load_shader_code(vertex_shader_code, vertex_shader_path);
    load_shader_code(fragment_shader_code, fragment_shader_path);

    return createShaderProgram(vertex_shader_code, fragment_shader_code);
}

// TEST CODE
// -------------------------
int test_main() {
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
