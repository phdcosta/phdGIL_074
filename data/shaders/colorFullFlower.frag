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

float flower(vec2 p) {
	float r=p.x*p.x+p.y*p.y;
	float a=atan(p.y,p.x);
	return r+sin((a-time)*5.0*params.x);
}

vec3 color(float c) {
	return vec3(sin(c-time),sin(c*2.1*params.y+1.0-time),sin(c*2.3*params.z+2.0+time));
}

void main() {
	vec2 p = gl_FragCoord.xy / resolution.xy - pos.xy;
	p.x *= (resolution.x / resolution.y);
	float c = flower(p);
	gl_FragColor = clampColor(color(c).xyz, clrLow, clrHigh);
}
