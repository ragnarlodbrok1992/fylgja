#version 330 core

// Input vertex attributes
layout(location = 0) in vec3 aPos; // Vertex position
layout(location = 1) in vec3 aColor; // Vertex color

// Output to fragment shader
out vec3 color;

void main()
{
    // Transform the vertex position using the MVP matrix
    gl_Position = vec4(aPos, 1.0);
    
    // Pass the color to the fragment shader
    color = aColor;
}
