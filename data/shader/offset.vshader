#version 330

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;

smooth out vec4 theColor;
uniform vec2 offset;

void main(){
	vec4 theOffset = vec4(offset, 0.0f, 0.0f);
	gl_Position = position + theOffset;
	theColor = color;
}
