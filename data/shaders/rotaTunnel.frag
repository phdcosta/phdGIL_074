#version 120

uniform vec2 resolution;
uniform vec2 mouse;
uniform float time;
uniform sampler2D tex0;
uniform vec4 clrBase;
uniform vec4 clrLow;
uniform vec4 clrHigh;
uniform vec4 params;
uniform vec2 pos;
uniform vec2 scale;

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

void main() {
	vec2 p = gl_FragCoord.xy / resolution.xy - pos.xy;
	p.x *= (resolution.x / resolution.y);
	vec2 uv;
	float r = sqrt( dot(p,p) );
	float a = atan(p.y,p.x) + 0.9*sin(0.5*r-0.5*time);
	float s = 0.5 + 0.5*cos(8.0*params.x*a);
	s = smoothstep(0.0,1.0,s);
	s = smoothstep(0.0,1.0,s);
	s = smoothstep(0.0,1.0,s);
	s = smoothstep(0.0,1.0,s);
	uv.x = time + 1.0/( r + 0.2*s);
	uv.y = 1.0*a/1000.1416;
	float w = (0.5 + 0.5*s)*r*r;
	float ao = 0.5 + 0.5*cos(42.0*params.y*a);
	ao = smoothstep(0.0,0.4,ao)-smoothstep(0.4,0.7,ao);
	ao = 1.0-0.5*ao*r;
	float px = gl_FragCoord.x/resolution.x;
	float py = gl_FragCoord.y/resolution.y;
	float x = mod(uv.x*resolution.x,resolution.x/3.5);
	float y = mod(uv.y*resolution.y+(resolution.y/2.),resolution.y/3.5);
	float v =  (x / y)-.7;
	gl_FragColor = clampColor(vec3(.8-v,.9-v,1.3-v)*w*ao, clrLow, clrHigh);
}
