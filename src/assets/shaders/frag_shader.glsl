#version 330 core

// Input data from vertex shader
in vec3 color;

// Output color
out vec4 fragColor;

void main()
{
    // Set the fragment color
    fragColor = vec4(color, 1.0); // Use the color from the vertex shader with full opacity
}
