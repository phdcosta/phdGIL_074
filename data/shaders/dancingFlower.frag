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

float u( float x ) {
	return (x>0.0)?1.0:0.0;
}

void main() {
	vec2 p = gl_FragCoord.xy / resolution.xy - mouse.xy / resolution.xy;
	p.xy /= .25;
	float a = atan(p.x,p.y);
	float r = length(p)*params.z;
	float w = cos(3.1415927*time-r*2.0);
	float h = 0.5+0.5*cos(params.x*20.0*a-w*20.0*params.y+r*8.0);
	float d = 0.25+0.75*pow(h,1.0*r)*(0.7+0.3*w);
	float col = u( d-r ) * sqrt(1.0-r/d)*r*5.*params.a;
	gl_FragColor = clampColor(vec3(col, col-h*0.5+r*.2 + 0.35*h*(1.0-r), col-h*r + 0.1*h*(1.0-r)), clrLow, clrHigh);
}
