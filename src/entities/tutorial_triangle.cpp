#include "tutorial_triangle.hpp"

TutorialTriangle::prepare() {
    render_data.vertices = triangle_vertices;
    render_data.prepare();
}

TutorialTriangle::render() {
    render_data.render();
}

TutorialTriangle::teardown() {
    render_data.teardown();
}
