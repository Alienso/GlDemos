#version 430 core
layout (location = 0) in vec2 aPos;
uniform vec3 aColor;

out vec3 ourColor;

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);
    ourColor = aColor;
}