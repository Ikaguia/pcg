#version 330

out vec4 outputColor;


#define POINT_LIGHT_COUNT 4
struct PointLight{
	vec4 position;
	vec4 color;
	float ambientStrength;
	float specularStrength;

	// float linearDecay;
	// float quadraticDecay;
};
uniform PointLight pLight;
// uniform PointLight lights[POINT_LIGHT_COUNT];
// uniform int lightCount;

uniform sampler2D theTexture;
uniform vec4 cameraPosition;
uniform vec4 objectColor;

smooth in vec4 modelPosition;
smooth in vec4 worldPosition;
smooth in vec2 fragTex;
smooth in vec3 fragNorm;


vec4 calcAmbient(PointLight l){
	return l.ambientStrength * l.color;
}
vec4 calcDiffuse(PointLight l){
	vec3 lightDir = normalize(l.position - worldPosition).xyz;
	float diff = max(dot(fragNorm, lightDir), 0.0f);
	return diff * l.color;
}
vec4 calcSpecular(PointLight l){
	vec3 lightDir = normalize(l.position - worldPosition).xyz;
	vec3 viewDir = normalize(cameraPosition - worldPosition).xyz;
	vec3 reflectDir = reflect(-lightDir, fragNorm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 16);
	return l.specularStrength * spec * l.color;
}

void main(){
	vec4 objColor = objectColor;
	// vec4 objColor = texture(theTexture, fragTex.xy);

	vec3 tmp = vec3(0.0f, 0.0f, 0.0f);
	tmp += calcAmbient(pLight).xyz;
	tmp += calcDiffuse(pLight).xyz;
	tmp += calcSpecular(pLight).xyz;
	// int end = min(lightCount, POINT_LIGHT_COUNT);
	// for(int i = 0; i < end; i++){
	// 	tmp += calcAmbient(lights[i]).xyz;
	// 	tmp += calcDiffuse(lights[i]).xyz;
	// 	tmp += calcSpecular(lights[i]).xyz;
	// }

	outputColor = vec4(tmp, 1.0f) * objColor;

	// outputColor = lights[0].ambientStrength * lights[0].color * objColor;
}
