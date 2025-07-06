#ifndef _RENDER_DATA_HPP
#define _RENDER_DATA_HPP

#include <glad/glad.h>

struct RenderData {
  GLfloat* vertices; // Array of vertices plus colors
  GLuint VBO;         // Vertex Buffer Object
  GLuint VAO;         // Vertex Array Object
  void prepare();
  void render();
  void teardown();
};


#endif // _RENDER_DATA_HPP
