#version 330

out vec4 outputColor;


#define POINT_LIGHT_COUNT 4
struct PointLight{
	vec4 position;
	vec4 color;
	float ambientStrength;

	float linearDecay;
	float quadraticDecay;
};
uniform PointLight lights[POINT_LIGHT_COUNT];
uniform int lightCount;

struct Material{
	sampler2D ambientTexture;
	sampler2D diffuseTexture;
	sampler2D specularTexture;
	float shinyness;
};
uniform Material material;
uniform vec3 objectColor;
uniform vec4 cameraPosition;

smooth in vec4 modelPosition;
smooth in vec4 worldPosition;
smooth in vec2 fragTex;
smooth in vec3 fragNorm;


vec4 calcAmbient(PointLight l){
	return l.color * (l.ambientStrength * texture(material.ambientTexture, fragTex));
}
vec4 calcDiffuse(PointLight l){
	vec3 lightDir = normalize(l.position - worldPosition).xyz;
	float diff = max(dot(fragNorm, lightDir), 0.0f);
	return l.color * (diff * texture(material.diffuseTexture, fragTex));
}
vec4 calcSpecular(PointLight l){
	// return vec4(0,0,0,0);
	vec3 lightDir = normalize(l.position - worldPosition).xyz;
	vec3 viewDir = normalize(cameraPosition - worldPosition).xyz;
	vec3 reflectDir = reflect(-lightDir, fragNorm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shinyness);
	return l.color * (spec * texture(material.specularTexture, fragTex));
}
float calcAttenuation(PointLight l){
	float dist = distance(worldPosition, l.position);
	return 1.0f / (1.0f + l.linearDecay * dist + l.quadraticDecay * dist * dist);
}

void main(){
	vec3 light = vec3(0.0f, 0.0f, 0.0f);
	int end = min(lightCount, POINT_LIGHT_COUNT);
	for(int i = 0; i < end; i++){
		vec3 curLight = vec3(0.0f, 0.0f, 0.0f);
		curLight += clamp(calcAmbient(lights[i]).xyz, vec3(0,0,0), vec3(1,1,1));
		if(lights[i].position.y > 0.0f){
			curLight += clamp(calcDiffuse(lights[i]).xyz, vec3(0,0,0), vec3(1,1,1));
			curLight += clamp(calcSpecular(lights[i]).xyz, vec3(0,0,0), vec3(1,1,1));
		}

		light += curLight;
		// light += curLight * calcAttenuation(lights[i]);
	}
	clamp(light.x, 0.0f, 1.0f);
	clamp(light.y, 0.0f, 1.0f);
	clamp(light.z, 0.0f, 1.0f);

	outputColor = vec4(light * objectColor, 1.0f);
}
