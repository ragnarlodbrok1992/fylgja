#ifndef _TUTORIAL_TRIANGLE_HPP
#define _TUTORIAL_TRIANGLE_HPP

#include "../rendering/render_data.hpp"

GLfloat triangle_vertices[] = {
    // Positions         // Colors
    0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Top vertex (red)
   -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom left vertex (green)
    0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // Bottom right vertex (blue)
};

struct TutorialTriangle {
  RenderData render_data; // Render data for the triangle
  void prepare() {};

  void render() {};

  void teardown() {};
};



#endif // _TUTORIAL_TRIANGLE_HPP
