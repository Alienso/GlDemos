#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in float baseHeight;

out vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 uColor;
uniform float uTime;

void main(){
    vec3 pos = aPos;
    pos.x = pos.x + (pos.y/12)*sin(uTime * (1 + baseHeight));
	gl_Position = projection * view * model * vec4(pos, 1.0f);
	aColor = (uColor + vec3(aPos.y/15)) / 2;
}