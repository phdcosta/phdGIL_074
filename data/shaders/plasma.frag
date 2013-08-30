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

void main( void ) {
	vec2 unipos = ( gl_FragCoord.xy / resolution.xy );
	vec2 centerpos = vec2(cos(time), sin(time));
	vec2 centerpos2 = vec2(cos(time/5.), sin(time/3.+2.));
	vec2 centerpos3 = vec2(cos(time/4.+2.), sin(time/9.+2.));
	float val1 = cos(8.*(distance(centerpos2, unipos)));
	float val2 = sin(8.*(distance(centerpos.x, unipos.x)));
	float val3 = sin(8.*(distance(centerpos3.y, unipos.y)));
	float val = (val1 + val2 + val3);
	gl_FragColor = clampColor(vec3(sin(val+time*5.), cos(val+time*3.), sin(val+.3+time*7.)), clrLow, clrHigh);
}

