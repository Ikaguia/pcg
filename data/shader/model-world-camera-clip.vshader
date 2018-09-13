#version 330

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;

smooth out vec4 theColor;

uniform mat4 modelToWorldMatrix;
uniform mat4 worldToCameraMatrix;
uniform mat4 cameraToClipMatrix;

void main(){
	// vec4 tmp = modelToWorldMatrix * position;
	// tmp = worldToCameraMatrix * tmp;
	// gl_Position = cameraToClipMatrix * tmp;
	gl_Position = cameraToClipMatrix * position;
	// gl_Position = position;
	theColor = color;
}
