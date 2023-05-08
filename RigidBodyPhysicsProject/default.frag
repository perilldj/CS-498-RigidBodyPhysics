#version 330 core
out vec4 FragColor;

in vec3 normal;
in vec2 texCoord;

uniform sampler2D tex0;
uniform vec4 objColor;

void main() {
	float value = max(dot(normal, normalize(vec3(1.0f, 0.0f, 0.0f))), 0.2);
	FragColor = objColor * value;
}