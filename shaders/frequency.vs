#version 430 core
layout (location = 0) in vec3 aPos;
uniform vec3 aColor;

out vec3 ourColor;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    ourColor = vec3(2 * aPos.y , 1 - 2 * aPos.y, 0);
}