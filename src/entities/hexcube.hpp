#ifndef _HEXCUBE_HPP
#define _HEXCUBE_HPP

#include <numbers>

// Math constants
constexpr double PI = std::numbers::pi;

// Hexcube defines
// Assumptions is here that hexcubes are flat topped (the one I really like)
constexpr int HEXCUBE_SIZE = 20;

// Hexcube declarations
typedef struct {
  vec2_int pos; // Top-left hex is [0, 0]
  vec2_double verts[6];
} Hexcube;

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

#endif // _HEXCUBE_HPP
