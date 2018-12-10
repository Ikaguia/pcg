#version 330

uniform vec3 objectColor;
out vec4 outputColor;

void main(){
	outputColor = vec4(objectColor, 1.0f);
}
