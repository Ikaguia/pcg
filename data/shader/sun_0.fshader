#version 330

out vec4 outputColor;

uniform sampler2D theTexture;
uniform vec4 cameraPosition;
uniform vec3 objectColor;

smooth in vec4 modelPosition;
smooth in vec4 worldPosition;
smooth in vec2 fragTex;

uniform float iTime;
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
	p.x = p.x * 3;
	p.y = p.y * 3;
	p.z = p.z * 3;
	p.x += sin(the_time/10000);
	p.y += cos(the_time/10000);
	p.z += the_time/10000;

	vec3 c;

	//arbitrary vector added to p,
	//f(p) = fbm( p + fbm( p + fbm(p) ) ) formula taken from http://www.iquilezles.org/www/articles/warp/warp.htm
	c.r = pow(fbm(p + vec3(2.13, 1.21, 3.32) + fbm(p + vec3(1.21, 3.32, 2.13) + fbm(p + vec3(3.32, 1.21, 2.13)))), 2.0) * 2.0;
	c.g = pow(fbm(p + vec3(3.11, 2.23, 1.32) + fbm(p + vec3(2.23, 1.32, 3.11) + fbm(p + vec3(1.32, 2.23, 3.11)))), 2.0) * 2.0;
	c.b = pow(fbm(p + vec3(3.12, 1.22, 2.31) + fbm(p + vec3(1.22, 2.31, 3.12) + fbm(p + vec3(2.31, 1.22, 3.12)))), 2.0) * 2.0;

	c.r = clamp((c.r + c.g) / 1.5, 0.0, 1.0);
	// c.g /= 3.0;
	c.b /= 5.0;
	return c;
}

void main(){
	outputColor = vec4(foo(modelPosition.xyz) * objectColor, 1.0f);
}
