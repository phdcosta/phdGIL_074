// by rotwang, overlapping pixels in different sizes

// drifting around by @emackey

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

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main( void ) {

	vec2 dir = vec2(cos(time * 0.1), sin(time * 0.1)) * 0.2;
	vec2 travel = time * 0.01 + (mouse/resolution) * 0.2;
	float aspect = resolution.x / resolution.y;
	vec2 unipos = ( gl_FragCoord.xy / resolution ) + dir + travel;
 	vec2 pos = unipos*2.0-1.0;//bipolar
	pos.x *= aspect;

	vec3 clr = vec3(0.0);
	for(int i=0;i<8;i++)
	{
		float n = pow(8.0-float(i), 2.0);
		vec2 pos_z = floor(pos*n);
		float a = 1.0-step(0.1, rand(pos_z));
		float rr = rand(pos_z)*a;
		float gg = rand(pos_z+n)*a;
		float bb = rand(pos_z+n+n)*a;
		
		vec3 clr_a = vec3(rr, gg, bb);
		clr += clr_a/2.0;
		pos += dir + travel;
	}
	
	
	gl_FragColor = clampColor( clr, clrLow, clrHigh );
	
}