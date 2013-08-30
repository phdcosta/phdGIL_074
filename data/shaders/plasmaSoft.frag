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

#ifdef GL_ES
precision highp float;
#endif

#define M_PI 3.1415926535897932384626433832795
#define N 11.0
#define ZOOM 50.0

float post_process(float color) {
	float m = mod(color, 2.0);
	if (m >= 1.0)
		color = 2.0 - m;
	else
		color = m;
	return color;
}

void main( void ) {

	// This is a reimplementation of this thing:
	// http://mainisusuallyafunction.blogspot.no/2011/10/quasicrystals-as-sums-of-waves-in-plane.html
	
	vec2 p = ( gl_FragCoord.xy ) / 2.0 + mouse * resolution*0.3;
	p /= ZOOM;
	p += vec2(20);

	vec3 color = vec3(0.0);
	float r = 0.0;
	float g = 0.0;
	float b = 0.0;

	for (float i = 0.0; i < N; ++i) {
		float a = i * (2.0 * M_PI / N);
		float t = cos((p.x * cos(a) + p.y * sin(a)) + time ) / 2.0;
		r += t + 0.5 + 0.2 * i;
		g += t + 1.0 + 0.4 * i;
		b += t + 1.5 + 0.6 * i;
	}
	
	r = post_process(r);
	g = post_process(g);
	b = post_process(b);

	gl_FragColor = vec4( r, g, b, 1.0 );

}
