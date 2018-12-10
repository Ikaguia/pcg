#version 330

out vec4 fragColor;


uniform sampler2D theTexture;
#define iChannel0 theTexture
uniform float iTime;
uniform vec4 iMouse;
uniform vec3 iResolution;

smooth in vec4 worldPosition;
#define fragCoord worldPosition


// based on https://www.shadertoy.com/view/lsf3RH by
// trisomie21 (THANKS!)
// My apologies for the ugly code.

#define uv0 (floor(mod(uv, res))*vec3(1e0, 1e2, 1e4))
#define uv1 (floor(mod(uv+vec3(1.), res))*vec3(1e0, 1e2, 1e4))

float snoise(vec3 uv, float res){	// by trisomie21
	uv *= res;

	// vec3 uv0 = floor(mod(uv, res))*vec3(1e0, 1e2, 1e4);
	// vec3 uv1 = floor(mod(uv+vec3(1.), res))*vec3(1e0, 1e2, 1e4);

	vec3 f = fract(uv); f = f*f*(3.0-2.0*f);

	vec4 v = vec4(uv0.x+uv0.y+uv0.z, uv1.x+uv0.y+uv0.z,
		      	  uv0.x+uv1.y+uv0.z, uv1.x+uv1.y+uv0.z);

	// #define r fract(sin(v*1e-3)*1e5)
	vec4 r = fract(sin(v*1e-3)*1e5);
	#define r0 mix(mix(r.x, r.y, f.x), mix(r.z, r.w, f.x), f.y)
	// float r0 = mix(mix(r.x, r.y, f.x), mix(r.z, r.w, f.x), f.y);

	#define r_ fract(sin((v + uv1.z - uv0.z)*1e-3)*1e5)
	// r = fract(sin((v + uv1.z - uv0.z)*1e-3)*1e5);
	#define r1 (mix(mix(r_.x, r_.y, f.x), mix(r_.z, r_.w, f.x), f.y))
	// float r1 = mix(mix(r.x, r.y, f.x), mix(r.z, r.w, f.x), f.y);

	return mix(r0, r1, f.z)*2.-1.;
}

// float brightness = 0.1;

#define time (iTime * 0.1)
#define aspect (iResolution.x / iResolution.y)
#define brightness 0.15
#define radius (0.24 + brightness * 0.2)
#define invRadius (1.0 / radius)
#define orange vec3( 0.2, 0.65, 0.5 )
#define orangeRed vec3( 0.1, 0.25, 0.81 )
#define fade pow( length( 2.0 * p ), 0.5 )
#define angle (atan( p.x, p.y )/6.2832)
#define dist length(p)
#define coord vec3( angle, dist, time * 0.1 )
#define newTime1 abs( snoise( coord + vec3( 0.0, -time * ( 0.35 + brightness * 0.001 ), time * 0.015 ), 15.0 ) )
#define newTime2 abs( snoise( coord + vec3( 0.0, -time * ( 0.15 + brightness * 0.001 ), time * 0.015 ), 45.0 ) )
#define starGlow min( max( 1.0 - dist * ( 1.0 - brightness ), 0.0 ), 1.0 )

void main(){
    // if( brightness < 0.15 ) {
    //     brightness	= max( ( cos(iTime) * 0.25 + sin(iTime) * 0.25 ), 0.1);
    // }
	// float radius		= 0.24 + brightness * 0.2;
	// float invRadius 	= 1.0/radius;

	// vec3 orange			= vec3( 0.2, 0.65, 0.5 );
	// vec3 orangeRed		= vec3( 0.1, 0.25, 0.81 );
	vec2 p 			= -0.5 + (fragCoord.xy / iResolution.xy);
	p.x *= aspect;

	// float fade		= pow( length( 2.0 * p ), 0.5 );
	float fVal1		= 1.0 - fade;
	float fVal2		= 1.0 - fade;

	// float angle		= atan( p.x, p.y )/6.2832;
	// float dist		= length(p);
	// vec3 coord		= vec3( angle, dist, time * 0.1 );

	// float newTime1	= abs( snoise( coord + vec3( 0.0, -time * ( 0.35 + brightness * 0.001 ), time * 0.015 ), 15.0 ) );
	// float newTime2	= abs( snoise( coord + vec3( 0.0, -time * ( 0.15 + brightness * 0.001 ), time * 0.015 ), 45.0 ) );
	for( int i=1; i<=7; i++ ){
		#define power pow( 2.0, float(i + 1) )
		// float power = pow( 2.0, float(i + 1) );
		fVal1 += ( 0.5 / power ) * snoise( coord + vec3( 0.0, -time, time * 0.2 ), ( power * ( 10.0 ) * ( newTime1 + 1.0 ) ) );
		fVal2 += ( 0.5 / power ) * snoise( coord + vec3( 0.0, -time, time * 0.2 ), ( power * ( 25.0 ) * ( newTime2 + 1.0 ) ) );
	}

	float corona		= pow( fVal1 * max( 1.1 - fade, 0.0 ), 2.0 ) * 50.0;
	corona				+= pow( fVal2 * max( 1.1 - fade, 0.0 ), 2.0 ) * 50.0;
	corona				*= 1.2 - newTime1;
	vec3 starSphere		= vec3( 0.0 );

	vec2 sp = -1.0 + 2.0 * (fragCoord.xy / iResolution.xy);
	sp.x *= aspect;
	sp *= ( 2.0 - brightness );
  	#define r dot(sp,sp)
  	// float r = dot(sp,sp);
	#define f ((1.0-sqrt(abs(1.0-r)))/(r) + brightness * 0.5)
	// float f = (1.0-sqrt(abs(1.0-r)))/(r) + brightness * 0.5;
	if( dist < radius ){
		corona			*= pow( dist * invRadius, 24.0 );
  		vec2 newUv;
 		newUv.x = sp.x*f;
  		newUv.y = sp.y*f;
		newUv += vec2( time, 0.0 );

		vec3 texSample 	= texture( iChannel0, newUv ).rgb;
		#define uOff ( texSample.g * brightness * 3.14 + time )
		// float uOff		= ( texSample.g * brightness * 3.14 + time );
		#define starUV (newUv + vec2( uOff, 0.0 ))
		// vec2 starUV		= newUv + vec2( uOff, 0.0 );
		starSphere		= texture( iChannel0, starUV ).rgb;
	}

	// float starGlow	= min( max( 1.0 - dist * ( 1.0 - brightness ), 0.0 ), 1.0 );
	//fragColor.rgb	= vec3( r );
	fragColor.rgb	= vec3( f * ( 0.75 + brightness * 0.3 ) * orange ) + starSphere + corona * orange + starGlow * orangeRed;
	fragColor.a		= 1.0;
}

