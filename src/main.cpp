#include <stdio.h>
#include <numbers>
#include <cmath>

// OpenGL headers
#include <glad/glad.h>

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_opengl.h>

// Local imports
#include "rendering/shaders/loading_shaders.hpp"


// Engine structures
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_GLContext context;

// OpenGL stuff
static GLuint shaderProgram;

constexpr const char* WINDOW_TITLE = "Fylgja - development phase version.";
constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 768;

// SDL Metadata values
constexpr const char* APP_NAME = "Fylgja";
constexpr const char* APP_VERSION = "0.0.1";
constexpr const char* APP_IDENTIFIER = "fylgja.gamedev.ragnar";

// Math constants
constexpr double PI = std::numbers::pi;

// Engine control data
// TODO - should we put it into appstate? It's passed around the callback functions
static bool ENGINE_RUN = true;

// Shaders for engine
const char* vertexShader =
R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out vec3 ourColor;

void main() {
  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
  ourColor = aColor;
}
)";

// Test entity - hello world triangle
struct TestEntity {
  GLfloat vertices[3 * 6] = { // Three vertices, each with 3 components (x, y, z) and 3 color components (r, g, b)
    // Positions          // Colors
    -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // Bottom left
     0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // Bottom right
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f  // Top
  };
  GLuint VBO, VAO; // Vertex Buffer Object and Vertex Array Object
  
  void prepare() {
    // Generate and bind the Vertex Array Object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generate and bind the Vertex Buffer Object
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Unbind the VAO (not the EBO) to avoid accidentally modifying it
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  void render() {
    // Bind the VAO and draw the triangle
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0); // Unbind the VAO
  }

  void teardown() {
    // Delete the VBO and VAO
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
  }
};

// Static test entity
static TestEntity testEntity;

const char* fragmentShader =
R"(
#version 330 core

in vec3 ourColor;
out vec4 FragColor;

void main() {
  FragColor = vec4(ourColor, 1.0f);
}
)";

// OpenGL shader functions
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


// Some color defines
static SDL_Color BACKGROUND = {0x27, 0x2a, 0x2e, 0xff};
static SDL_Color WHITE = {0xff, 0xff, 0xff, 0xff};
static SDL_Color MISSING_TEXTURE_PURPLE = {0xff, 0x00, 0xdc, 0xff};
static SDL_Color HEX_CUBE_BORDER = {0xb3, 0x8b, 0x79, 0xff};

typedef struct {
  GLclampf r;
  GLclampf g;
  GLclampf b;
  GLclampf a;
} ogl_color;

constexpr float uint8_to_float(uint8_t value) {
  return static_cast<float>(value) / 0xff;
}

static ogl_color GL_BACKGROUND = {
  .r = uint8_to_float(0x27),
  .g = uint8_to_float(0x2a),
  .b = uint8_to_float(0x2e),
  .a = uint8_to_float(0xff)
};

// Linear algebra primitives
typedef struct {
  int x;
  int y;
} vec2_int;

typedef struct {
  double x;
  double y;
} vec2_double;

// Hexcube defines
// Assumptions is here that hexcubes are flat topped (the one I really like)
constexpr int HEXCUBE_SIZE = 20;

// Hexmap - collection of hexcubes for now it's 2d TODO: creating 3d maps?
static vec2_int HEXMAP_TOP_LEFT = {100, 100};

// Hexcube declarations
typedef struct {
  vec2_int pos; // Top-left hex is [0, 0]
  vec2_double verts[6];
} Hexcube;

// Static objects to test stuff
static Hexcube test_hexcube;

// Hexcube functions
vec2_double flat_hex_corner(vec2_int center, int size, int i) {
  int angle_degrees = 60 * i; // Wraps around in 6 iterations, cuz degrees you know TODO add some asserts maybe
  double angle_radians = PI / 180 * static_cast<double>(angle_degrees);
  return {center.x + size * cos(angle_radians), center.y + size * sin(angle_radians)};
}

void hexcube_populate_verts(Hexcube* hexcube) {
  // Function that takes pointer to hexcube and prepares it's vertices
  // Verts are calculated based on hex pos and top-left of hexmap
  for (int i = 0; i < 6; i++) {
    hexcube->verts[i] = flat_hex_corner(hexcube->pos, HEXCUBE_SIZE, i);
  }
}

void print_hexcube_verts(Hexcube* hexcube) {
  printf("print_hexcube_verts for hexcube --> %p\n", hexcube);
  for (int i = 0; i < 6; i++) {
    printf(" [%d] --> x: %f, y: %f\n", i, hexcube->verts[i].x, hexcube->verts[i].y);
  }
}

// TODO remove - old 2d api
void render_hexcube(Hexcube* hexcube, SDL_Color color) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

  for (int i = 0; i < 5; i++) {
    SDL_RenderLine(renderer,
        (float)hexcube->verts[i].x,
        (float)hexcube->verts[i].y,
        (float)hexcube->verts[i + 1].x,
        (float)hexcube->verts[i + 1].y);
  }
  SDL_RenderLine(renderer,
      (float)hexcube->verts[0].x,
      (float)hexcube->verts[0].y,
      (float)hexcube->verts[5].x,
      (float)hexcube->verts[5].y);
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  SDL_SetAppMetadata(APP_NAME, APP_VERSION, APP_IDENTIFIER);

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS,
        &window, &renderer)) {
    SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  context = SDL_GL_CreateContext(window);
  if (!context) {
    SDL_Log("OpenGL context could not be created! SDL_Error: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_GL_MakeCurrent(window, context)) {
    SDL_Log("SDL_GL_MakeCurrent failed: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  // Initialize GLAD
  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
    SDL_Log("Failed to initialize GLAD");
    return SDL_APP_FAILURE;
  }

  SDL_GL_SetSwapInterval(1);

  // Test code - testing static objects - hexcube
  // test_hexcube.pos = HEXMAP_TOP_LEFT;
  // print_hexcube_verts(&test_hexcube);
  // hexcube_populate_verts(&test_hexcube);
  // print_hexcube_verts(&test_hexcube);
  
  // Create shader program and prepare test entity
  shaderProgram = createShaderProgram(vertexShader, fragmentShader);
  if (shaderProgram == 0) {
    SDL_Log("Failed to create shader program");
    return SDL_APP_FAILURE;
  }
  testEntity.prepare();

  return SDL_APP_CONTINUE;
}

// Input stuff
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  switch (event->type) {
    // Should exit app?
    case SDL_EVENT_QUIT:
      return SDL_APP_SUCCESS;
      break;
    // Check keyboard events
    case SDL_EVENT_KEY_DOWN:
      // printf("Key pressed!\n");
      switch (event->key.scancode) {
        case SDL_SCANCODE_Q:
          ENGINE_RUN = false; // Stopping engine
        default:
          break;
      }
      break;
    default:
      break;
    }

  return SDL_APP_CONTINUE;
}

// Gameloop stuff
SDL_AppResult SDL_AppIterate(void *appstate) {
  // Check for ENGINE_RUN
  if (!ENGINE_RUN) return SDL_APP_SUCCESS;

  glClearColor(GL_BACKGROUND.r, GL_BACKGROUND.g, GL_BACKGROUND.b, GL_BACKGROUND.a);
  glClear(GL_COLOR_BUFFER_BIT);

  // Rendering goes here
  glUseProgram(shaderProgram);
  testEntity.render();

  SDL_GL_SwapWindow(window);

  return SDL_APP_CONTINUE;
}

// Teardown stuff goes here
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  // Cleanup Test Entity
  testEntity.teardown();
}
