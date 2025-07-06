#ifndef _TUTORIAL_TRIANGLE_HPP
#define _TUTORIAL_TRIANGLE_HPP

#include <glad/glad.h>

#include "../rendering/render_data.hpp"

extern GLfloat triangle_vertices[];

struct TutorialTriangle {
  RenderData render_data; // Render data for the triangle
  void prepare();

  void render();

  void teardown();
};



#endif // _TUTORIAL_TRIANGLE_HPP
