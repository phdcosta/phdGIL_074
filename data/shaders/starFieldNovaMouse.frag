#version 120

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

#ifdef GL_ES
precision mediump float;
#endif

// Posted by Trisomie21

// from http://glsl.heroku.com/e#5248.0
#define BIAS 0.1
#define SHARPNESS 3.0
vec3 star(vec2 position, float BLADES) {
	float blade = clamp(pow(sin(atan(position.y,position.x )*BLADES)+BIAS, SHARPNESS), 0.0, 1.0);
	vec3 color = mix(vec3(-0.34, -0.5, -1.0), vec3(0.0, -0.5, -1.0), (position.y + 1.0) * 0.25);
	float d = 1.0/length(position) * 0.075;
	color += vec3(0.95, 0.65, 0.30) * d;
	color += vec3(0.95, 0.45, 0.30) * min(1.0, blade *0.7) *  d;
	return color;
}

vec3 star2(vec2 position, float BLADES) {
	float blade = clamp(pow(sin(atan(position.y,position.x )*BLADES + time*.5)+BIAS, 8.0), 0.0, 1.0);
	vec3 color = mix(vec3(-0.34, -0.5, -0.0), vec3(0.0, -0.5, -0.0), (position.y + 1.0) * 0.25);
	float d = 1.0/length(position) * 0.075;
	color += vec3(0.95, 0.65, 0.30) * d;
	color += vec3(0.95, 0.45, 0.30) * min(1.0, blade *0.7)*0.5;
	return max(color.rgb, 0.0)*.5;
}


// Tweaked from http://glsl.heroku.com/e#4982.0
float hash( float n ) { return fract(sin(n)*43758.5453); }

float noise( in vec2 x )
{
	vec2 p = floor(x);
	vec2 f = fract(x);
    	f = f*f*(3.0-2.0*f);
    	float n = p.x + p.y*57.0;
    	float res = mix(mix(hash(n+0.0), hash(n+1.0),f.x), mix(hash(n+57.0), hash(n+58.0),f.x),f.y);
    	return res;
}

vec3 cloud(vec2 p) {
	vec3 f = vec3(0.0);
    	f += 0.5000*noise(p*10.0)*vec3(.45, .55, 1.0);
    	f += 0.2500*noise(p*20.0)*vec3(.85, .45, 1.0);
    	f += 0.1250*noise(p*40.0)*vec3(1.0, .00, 0.3);
    	f += 0.0625*noise(p*80.0)*vec3(1.0, 1.0, 1.0);	
	return f*.5;
}

const float SPEED	= -0.005;
const float SCALE	= 80.0;
const float DENSITY	= 1.5;
const float BRIGHTNESS	= 10.0;
       vec2 ORIGIN	= resolution.xy*.5;

float rand(vec2 co){ return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453); }

vec3 layer(float i, vec2 pos, float dist, vec2 coord) {
	float a = pow((1.0-dist),20.0);
	float t = i*10.0 - time*i*i;
	float r = coord.x - (t*SPEED);
	float c = fract(a+coord.y + i*.543);
	vec2  p = vec2(r, c*.5)*SCALE*(4.0/(i*i));
	vec2 uv = fract(p)*2.0-1.0;
	float m = clamp((rand(floor(p))-DENSITY/i)*BRIGHTNESS, 0.0, 1.0);
	return  clamp(star(uv*.5, 6.0)*m*dist, 0.0, 1.0);
}

void main( void ) {
	
	vec2   pos = gl_FragCoord.xy - mouse.xy;
	float dist = length(pos) / resolution.x;
	vec2 coord = vec2(pow(dist, 0.1), atan(pos.x, pos.y) / (3.1415926*2.0));
	
	// Nebulous cloud
	vec3 color = cloud(pos/resolution);
	
	// Background stars
	float a = pow((1.0-dist),20.0);
	float t = time*-.05;
	float r = coord.x - (t*0.005);
	float c = fract(a+coord.y + 0.0*.543);
	vec2  p = vec2(r, c*.5)*4000.0;
	vec2 uv = fract(p)*2.0-1.0;
	float m = clamp((rand(floor(p))-.9)*10.0, 0.0, 1.0);
	color +=  clamp((1.0-length(uv*2.0))*m*dist, 0.0, 1.0);

	
	// Flying stars into black hole
	color += layer(2.0, pos, dist, coord);
	color += layer(3.0, pos, dist, coord);
	color += layer(4.0, pos, dist, coord);
	
	color += star2(pos/resolution, 2.0);
		
	gl_FragColor = vec4(color, 1.0);
}