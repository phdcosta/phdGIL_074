#version 120

#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 resolution;
uniform vec2 mouse;
uniform float time;
uniform sampler2D tex0;
uniform vec4 clrBase;
uniform vec4 clrLow;
uniform vec4 clrHigh;
uniform vec4 params;

vec4 clampColor(vec3 _clr, vec4 _low, vec4 _high) {
	_clr = clamp(mix(_clr.xyz, clrBase.xyz, clrBase.a), 0.0, 1.0);
	if((_clr.r >= _low.r && _clr.r <= _high.r) &&
		(_clr.g >= _low.g && _clr.g <= _high.g) &&
		(_clr.b >= _low.g && _clr.b <= _high.b)) {
		return vec4(_clr.xyz, _high.a);
	} else {
		return vec4(_clr.xyz, _low.a);
	}
}

// Posted by Trisomie21

// from http://glsl.heroku.com/e#5248.0
#define BLADES 6.0
#define BIAS 0.1
#define SHARPNESS 3.0
vec3 star(vec2 position) {
	float blade = clamp(pow(sin(atan(position.y,position.x )*BLADES)+BIAS, SHARPNESS), 0.0, 1.0);
	vec3 color = mix(vec3(-0.34, -0.5, -1.0), vec3(0.0, -0.5, -1.0), (position.y + 1.0) * 0.25);
	color += (vec3(0.95, 0.65, 0.30) * 1.0 / distance(vec2(0.0), position) * 0.075);
	color += vec3(0.95, 0.45, 0.30) * min(1.0, blade *0.7) * (1.0 / distance(vec2(0.0, 0.0), position)*0.075);
	return color;

}
const float LAYERS	= 5.0;
const float SPEED	= 0.005;
const float SCALE	= 40.0;
const float DENSITY	= 0.9;
const float BRIGHTNESS	= 10.0;
       vec2 ORIGIN	= resolution.xy*.5;

float rand(vec2 co){ return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453); }

void main( void ) {
	
	vec2   pos = gl_FragCoord.xy - ORIGIN;
	float dist = length(pos) / resolution.y;
	vec2 coord = vec2(pow(dist, 0.1), atan(pos.x, pos.y) / (3.1415926*2.0));
	
	vec3 color = vec3(0.0);
	
	float a = pow((1.0-dist),20.0);
	float t = time*-.05;
	float r = coord.x - (t*SPEED);
	float c = fract(a+coord.y + 0.0*.543);
	vec2  p = vec2(r, c*.5)*4000.0;
	vec2 uv = fract(p)*2.0-1.0;
	float m = clamp((rand(floor(p))-.9)*BRIGHTNESS, 0.0, 1.0);
	color +=  clamp((1.0-length(uv*2.0))*m*dist, 0.0, 1.0);
	
	for (float i = 1.0; i < (LAYERS+1.0); ++i)
	{
		float a = pow((1.0-dist),20.0);
		float t = i*10.0 - time*i*i;
		float r = coord.x - (t*SPEED);
		float c = fract(a+coord.y + i*.543);
		vec2  p = vec2(r, c*.5)*SCALE*(LAYERS/(i*i));
		vec2 uv = fract(p)*2.0-1.0;
		float m = clamp((rand(floor(p))-DENSITY/i)*BRIGHTNESS, 0.0, 1.0);
		color +=  clamp(star(uv*.5)*m*dist, 0.0, 1.0);
	}

	//color = mix( color, texture2D(backbuffer, gl_FragCoord.xy/resolution).rgb, min(dist+.1, 0.9));	

	gl_FragColor = vec4(color, 1.0);
}