#version 120

#ifdef GL_ES
precision mediump float;
#endif

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

#define BLADES 20.0
#define BIAS 1.0
#define SHARPNESS 4.0
#define COLOR 0.54, 0.92, 0.96
#define BG 0.94, 0.52, 0.26

void main( void ) {

	vec2 position = (( gl_FragCoord.xy / resolution.xy ) - vec2(mouse.x / resolution.x, mouse.y / resolution.y)) / vec2(resolution.y/resolution.x,1.0);
	vec3 color = vec3(0.);
	
	float blade = clamp(pow(sin(time+atan(position.y,position.x)*BLADES)+BIAS, SHARPNESS), 0.0, 1.0);
	
	color = mix(vec3(COLOR), vec3(BG), blade);
	
	gl_FragColor = vec4( color, 1.0 );

}