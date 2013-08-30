#version 120

#ifdef GL_ES
precision mediump float;
#endif

// Posted by Trisomie21

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

#define LAYERS 6.0

float rand(vec2 co){ return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453); }

void main( void ) {
	
	float count = 4.0;
	vec3 color = vec3(0.0);
	
	vec2 pos = gl_FragCoord.xy - resolution.xy*.5;
	float dist = length(pos) / resolution.y;
	
	for (float i = 0.0; i < LAYERS; ++i)
	{
		float t = time*(1.0+i*i);
		
		vec2 uv = vec2(dist, atan(pos.x, pos.y) / (3.1416*2.0));
		uv *= (i+1.0);
		
		vec2 p = uv;
		
		p.x = pow(uv.x, .1) - (t*.01);
		p.y /= 2.0;
		
		float r = pow(uv.x, .1) - (t*.01);
		
		uv.x = mod(r, 0.01)/.01;
		uv.y = mod(uv.y, 0.02)/.02;
		
		uv = uv*2.0-1.0;
		float a = 1.0-length(uv);
	
		vec3 m = fract(r*200.0 * vec3(.5, 1.0, -.25)*0.5)*.25+i*.1;
		
		p = floor(p*100.0);
		float d = (rand(p)-0.8)*20.0;
		d = clamp(d*dist, 0.0, 2.0);
	
		color = max(color, clamp(a*m*d, 0.0, 1.0));
	}

	gl_FragColor =  vec4(color, 1.0 );
}