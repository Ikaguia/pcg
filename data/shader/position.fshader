#version 330

out vec4 outputColor;

void main(){
	// float lerpValue = gl_FragCoord.y / 500.0f;

	// outputColor = mix(vec4(1.0f, 1.0f, 1.0f, 1.0f),
	//     vec4(0.2f, 0.2f, 0.2f, 1.0f), lerpValue);
	outputColor = vec4(
		asin(sin(gl_FragCoord.x / 100.0f) * cos(gl_FragCoord.y / 100.0f) /** tan(gl_FragCoord.z / 100.0f)*/),
		acos(cos(gl_FragCoord.x / 100.0f) * tan(gl_FragCoord.y / 100.0f) /** sin(gl_FragCoord.z / 100.0f)*/),
		atan(tan(gl_FragCoord.x / 100.0f) * sin(gl_FragCoord.y / 100.0f) /** cos(gl_FragCoord.z / 100.0f)*/),
		1.0f
	);
}
