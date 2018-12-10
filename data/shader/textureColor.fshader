#version 330

out vec4 outputColor;

smooth in vec4 theTexCoords;
smooth in vec4 theColor;
uniform sampler2D theTexture;

void main(){
	vec4 texColor = texture(theTexture, theTexCoords.xy);
	outputColor = 0.25*theColor + 0.25*texColor + 0.5*theColor*texColor;
}
