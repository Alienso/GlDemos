#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 uColor;
uniform float uTime;

void main()
{
    vec3 pos = aPos;
    pos.x = pos.x + (pos.y/6)*sin(uTime);
	gl_Position = projection * view * model * vec4(pos, 1.0f);
	aColor = (uColor + vec3(aPos.y/5)) / 2;
}