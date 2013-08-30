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

// pimped by @thevaw

#ifdef GL_ES
precision highp float;
#endif

vec4 pal(float t) {
	return vec4(
		sin(t/2.0+1.0)*0.5+0.6+cos(t/5.76+14.5)*0.5+0.5,
		(sin(t/2.0+2.1)*0.5+0.5+cos(t/4.76+14.5)*0.5+0.4)*0.6,
		(sin(t/2.0+3.2)*0.5+0.5+cos(t/3.76+14.5)*0.5+0.3)*0.4,
		1.0);
}

void main( void ) {
	vec2 pos = gl_FragCoord.xy / resolution;
	float aspect = resolution.x / resolution.y;
	vec2 mPos = mouse / resolution.xy;

	float rand = mod(fract(sin(dot(pos + time, vec2(12.9898,100.233))) * 43758.5453), 1.0) * 0.1;
	float col=0.0;
	col += .8 * (1. - (length((pos - (1.0 -mPos)) * vec2(aspect, 1.)) * 8.));
        col *= .8 * (1. - (length((pos - mPos) * vec2(aspect, 1.)) * (2.0+sin(time*0.5)*20.0)));

	//gl_FragColor = vec4( sin(rand*4.0), cos(rand*0.3), sin(10.0+rand*10.0), 1.0);
	vec4 color = pal(col/2.0+rand*20.0);
	gl_FragColor = clampColor(color.rgb, clrLow, clrHigh);
}