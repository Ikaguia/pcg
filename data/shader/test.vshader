#version 330

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;

smooth out vec4 theColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 clipMatrix;

void main(){
	vec4 tmp = position;
	tmp = modelMatrix * tmp;
	tmp = viewMatrix * tmp;
	tmp = clipMatrix * tmp;
	gl_Position = tmp;
	theColor = color;
}
