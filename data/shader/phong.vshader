#version 330

layout (location = 0) in vec4 xyzw;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;


uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 clipMatrix;


smooth out vec4 modelPosition;
smooth out vec4 worldPosition;
smooth out vec4 viewPosition;
smooth out vec2 fragTex;
smooth out vec3 fragNorm;


void main(){
	modelPosition = xyzw;
	worldPosition = modelMatrix * modelPosition;
	viewPosition = viewMatrix * worldPosition;
	gl_Position = clipMatrix * viewPosition;

	fragTex = tex;
	fragNorm = normalize(mat3(modelMatrix) * norm);
}
