#include "render_data.hpp"

// TODO: We are not taking into account the size of the vertices array here which is dynamic
RenderData::prepare() {
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
RenderData::render() {
    // Bind the VAO and draw the triangle
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0); // Unbind the VAO
}
RenderData::teardown() {
    // Delete the VBO and VAO
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

