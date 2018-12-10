#version 330

out vec4 outputColor;

uniform float the_time;

//The following functions were taken from https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83
float rand(vec2 co){ return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453); }
float mod289(float x){ return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec4 mod289(vec4 x){ return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec4 perm(vec4 x){ return mod289(((x * 34.0) + 1.0) * x); }
float noise(vec3 p){
	vec3 a = floor(p);
	vec3 d = p - a;
	d = d * d * (3.0 - 2.0 * d);

	vec4 b = a.xxyy + vec4(0.0, 1.0, 0.0, 1.0);
	vec4 k1 = perm(b.xyxy);
	vec4 k2 = perm(k1.xyxy + b.zzww);

	vec4 c = k2 + a.zzzz;
	vec4 k3 = perm(c);
	vec4 k4 = perm(c + 1.0);

	vec4 o1 = fract(k3 * (1.0 / 41.0));
	vec4 o2 = fract(k4 * (1.0 / 41.0));

	vec4 o3 = o2 * d.z + o1 * (1.0 - d.z);
	vec2 o4 = o3.yw * d.x + o3.xz * (1.0 - d.x);

	return o4.y * d.y + o4.x * (1.0 - d.y);
}
#define NUM_OCTAVES 5
float fbm(vec3 x) {
	float v = 0.0;
	float a = 0.5;
	vec3 shift = vec3(100);
	for (int i = 0; i < NUM_OCTAVES; ++i) {
		v += a * noise(x);
		x = x * 2.0 + shift;
		a *= 0.5;
	}
	return v;
}
//

vec3 foo(vec3 p){
	float pl = length( p );
	p.x += sin(the_time/20000);
	p.y += cos(the_time/20000);
	p.z += the_time/30000;

	vec3 c;

	//arbitrary vector added to p,
	//f(p) = fbm( p + fbm( p + fbm(p) ) ) formula taken from http://www.iquilezles.org/www/articles/warp/warp.htm
	c.r = pow(fbm(p + vec3(2.13, 1.21, 3.32) + fbm(p + vec3(1.21, 3.32, 2.13) + fbm(p + vec3(3.32, 1.21, 2.13)))), 2.0) * 2.0;
	c.g = pow(fbm(p + vec3(3.11, 2.23, 1.32) + fbm(p + vec3(2.23, 1.32, 3.11) + fbm(p + vec3(1.32, 2.23, 3.11)))), 2.0) * 2.0;
	c.b = pow(fbm(p + vec3(3.12, 1.22, 2.31) + fbm(p + vec3(1.22, 2.31, 3.12) + fbm(p + vec3(2.31, 1.22, 3.12)))), 2.0) * 2.0;
	return c;
}


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


vec4 calcAmbient(PointLight l){
	return l.ambientStrength * l.color;
}
vec4 calcDiffuse(PointLight l){
	return l.color;
	vec3 lightDir = normalize(l.position - worldPosition).xyz;
	float diff = max(dot(normalize(fragNorm), lightDir), 0.0f);
	return diff * l.color;
}
vec4 calcSpecular(PointLight l){
	// return vec4(0);
	vec3 lightDir = normalize(l.position - worldPosition).xyz;
	vec3 viewDir = normalize(cameraPosition - worldPosition).xyz;
	vec3 reflectDir = reflect(-lightDir, normalize(fragNorm));

	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 128);
	return l.specularStrength * spec * l.color;
}
float calcAttenuation(PointLight l){
	// return 1.0f;
	float dist = distance(worldPosition, l.position);
	return 1.0f / (1.0f + l.linearDecay * dist + l.quadraticDecay * dist * dist);
}

void main(){
	vec4 objColor = vec4(foo(modelPosition.xyz), 1.0f) * objectColor;
	// vec4 objColor = texture(theTexture, fragTex) * objectColor;

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
