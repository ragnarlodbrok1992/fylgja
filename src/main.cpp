#include <stdio.h>
#include <numbers>
#include <cmath>

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

// Engine structures
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

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

// Some color defines
static SDL_Color BACKGROUND = {0x27, 0x2a, 0x2e, 0xff};
static SDL_Color WHITE = {0xff, 0xff, 0xff, 0xff};
static SDL_Color MISSING_TEXTURE_PURPLE = {0xff, 0x00, 0xdc, 0xff};
static SDL_Color HEX_CUBE_BORDER = {0x52, 0x1b, 0x04, 0xff};

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

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  SDL_SetAppMetadata(APP_NAME, APP_VERSION, APP_IDENTIFIER);

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
    SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  // Test code - testing static objects - hexcube
  print_hexcube_verts(&test_hexcube);
  hexcube_populate_verts(&test_hexcube);
  print_hexcube_verts(&test_hexcube);

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

  // Frame draw here
  SDL_SetRenderDrawColor(renderer, BACKGROUND.r, BACKGROUND.g, BACKGROUND.b, BACKGROUND.a);
  SDL_RenderClear(renderer);

  // Rendering here

  SDL_RenderPresent(renderer);

  return SDL_APP_CONTINUE;
}

// Teardown stuff goes here
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
}
