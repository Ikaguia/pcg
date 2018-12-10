#version 330

smooth in vec4 worldPosition;
smooth in vec4 modelPosition;

out vec4 outputColor;

void main(){
	outputColor = vec4(
		0.025f + clamp(modelPosition.x, 0.0f, 1.0f),
		0.025f + clamp(modelPosition.y, 0.0f, 1.0f),
		0.025f + clamp(modelPosition.z, 0.0f, 1.0f),
		1.0f
	);
}
