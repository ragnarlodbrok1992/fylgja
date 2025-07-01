#include <stdio.h>
#include <cmath>

// OpenGL headers
#include <glad/glad.h>

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_opengl.h>

// Local imports
#include "rendering/render_defaults.hpp"

// Engine structures
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_GLContext context;

// Engine constants
constexpr const char* WINDOW_TITLE = "Fylgja - development phase version.";
constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 768;

// SDL Metadata values
constexpr const char* APP_NAME = "Fylgja";
constexpr const char* APP_VERSION = "0.0.1";
constexpr const char* APP_IDENTIFIER = "fylgja.gamedev.ragnar";


// Engine control data
// TODO - should we put it into appstate? It's passed around the callback functions
static bool ENGINE_RUN = true;

// Some color defines
static SDL_Color BACKGROUND = {0x27, 0x2a, 0x2e, 0xff};
static SDL_Color WHITE = {0xff, 0xff, 0xff, 0xff};
static SDL_Color MISSING_TEXTURE_PURPLE = {0xff, 0x00, 0xdc, 0xff};
static SDL_Color HEX_CUBE_BORDER = {0xb3, 0x8b, 0x79, 0xff};


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

  SDL_GL_SwapWindow(window);

  return SDL_APP_CONTINUE;
}

// Teardown stuff goes here
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
}
