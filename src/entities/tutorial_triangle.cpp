#include "tutorial_triangle.hpp"

GLfloat triangle_vertices[] = {
    // Positions         // Colors
    0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Top vertex (red)
   -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom left vertex (green)
    0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // Bottom right vertex (blue)
};

void TutorialTriangle::prepare() {
    render_data.vertices = triangle_vertices;
    render_data.vertex_count = 3;
    render_data.prepare();
}

void TutorialTriangle::render() {
    render_data.render();
}

void TutorialTriangle::teardown() {
    render_data.teardown();
}
