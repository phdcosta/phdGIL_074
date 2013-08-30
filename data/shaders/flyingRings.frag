#version 120

uniform float time;
uniform float alpha;
uniform vec2 mouse;
uniform vec2 resolution;
uniform vec4 clrBase;
uniform vec4 clrLow;
uniform vec4 clrHigh;
uniform vec4 params;
uniform vec4 fftA;
uniform vec4 fftB;
uniform vec2 pos;
uniform vec2 scale;
uniform sampler2DRect tex0;

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

float getValue(vec2 p, vec2 p2, float cmin, float cmax) {
	p -= p2;
	float r=(cmax-cmin) + cmin;
	return abs(r-length(p));
}

void main( void ) {
	vec2 position = gl_FragCoord.xy / resolution.xy - mouse.xy / resolution.xy;
	position.x *= (resolution.x / resolution.y);
	float sint = sin(time);
	float cost = cos(time);
	float color = 0.0;
	color += getValue(position, vec2(sint*.1, sint*.4), .4, .5);
	color *= getValue(position, vec2(cost*.2, cost*.3), .4, .5);
	color *= getValue(position, vec2(cost*.3, sint*.2), .4, .5);
	color *= getValue(position, vec2(sint*.4, cost*.1), .4, .5);
	color = 1./color*.00001;
	gl_FragColor = clampColor( vec3( color*.1, color*.01, color ), clrLow, clrHigh );
}
