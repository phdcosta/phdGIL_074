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

void main( void ) {

	vec2 position = gl_FragCoord.xy/resolution.y-vec2((resolution.x/resolution.y)/2.0,0.5);
	float value = sin(1.0-distance(position,vec2(0,0))*32.0+time*8.0)*0.5+0.5;
	position.y+=sin(position.x*4.0+time)*0.2;
	position.x+=cos(time/1.238374);
	float value2=sin(1.0-length(position)*32.0+time*8.0)*0.5+0.5;
	position.x+=sin(position.y*5.0+time)*0.2;
	position.y+=sin(time/1.6756);
	float value3=sin(1.0-length(position)*32.0+time*8.0)*0.5+0.5;
	value;
	gl_FragColor = clampColor(vec3(value3,value2-value3,value-length(value3+value2)), clrLow, clrHigh);
}
