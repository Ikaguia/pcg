#version 330

out vec4 outputColor;


#define POINT_LIGHT_COUNT 4
struct PointLight{
	vec4 position;
	vec4 color;
	float ambientStrength;
	float specularStrength;

	float linearDecay;
	float quadraticDecay;
};
uniform PointLight lights[POINT_LIGHT_COUNT];
uniform int lightCount;

uniform sampler2D theTexture;
uniform vec4 cameraPosition;
uniform vec4 objectColor;

smooth in vec4 modelPosition;
smooth in vec4 worldPosition;
smooth in vec2 fragTex;
smooth in vec3 fragNorm;


vec3 norm = normalize(fragNorm);

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

	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
	return l.specularStrength * spec * l.color;
}
float calcAttenuation(PointLight l){
	float dist = distance(worldPosition, l.position);
	return 1.0f / (1.0f + l.linearDecay * dist + l.quadraticDecay * dist * dist);
}

void main(){
	vec4 objColor = texture(theTexture, fragTex) * objectColor;

	vec3 light = vec3(0.0f, 0.0f, 0.0f);
	int end = min(lightCount, POINT_LIGHT_COUNT);
	for(int i = 0; i < end; i++){
		vec3 curLight = vec3(0.0f, 0.0f, 0.0f);
		curLight += calcAmbient(lights[i]).xyz;
		curLight += calcDiffuse(lights[i]).xyz;
		curLight += calcSpecular(lights[i]).xyz;

		light += curLight * calcAttenuation(lights[i]);
	}
	clamp(light.x, 0.0f, 1.0f);
	clamp(light.y, 0.0f, 1.0f);
	clamp(light.z, 0.0f, 1.0f);

	outputColor = vec4(light, 1.0f) * objColor;
}
