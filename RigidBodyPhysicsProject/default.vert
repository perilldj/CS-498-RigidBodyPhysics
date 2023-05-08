#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

out vec3 normal;

out vec2 texCoord;

uniform float scale;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform mat4 camMatrix;

void main() {
	gl_Position = proj * (camMatrix * view) * model * vec4(aPos, 1.0);
	normal = aNormal;
	texCoord = aTex;
}