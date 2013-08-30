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

#define M_PI 3.1415926535897932384626433832795
#define N 9.0
#define ZOOM 1.0
#define SPEED 0.5

float post_process(float color) {
	float m = mod(color, 2.0);
	if (m >= 1.0)
		color = 2.0 - m;
	else
		color = m;
	return color;
}

vec2 complex_div(vec2 numerator, vec2 denominator){
   return vec2( numerator.x*denominator.x + numerator.y*denominator.y,
                numerator.y*denominator.x - numerator.x*denominator.y)/
          vec2(denominator.x*denominator.x + denominator.y*denominator.y);
}

vec3 sun( vec2 pos ) {
	vec2 p = mouse.xy / resolution.xy;
	return vec3(2.5, 3.5, 6.0) / (distance( p, pos ) * 10.0);
}

void main( void ) {
	vec2 aspect = vec2(1.0, resolution.y/resolution.x);
	vec2 p = (( gl_FragCoord.xy / resolution.xy )) * aspect;
	p = mix( -0.5 + (p- 0.5)*8., complex_div(vec2(0.25,.0), p - mouse/resolution.xy*aspect) + 0.5, 0.5); 
	p /= ZOOM;
	p += vec2(20);

	vec3 color = vec3(0.0);
	float r = 0.0;
	float g = 0.0;
	float b = 0.0;
	//edit: manually unrolled the loop for my obviously broken shader compilers // fnord
	//edit2: index correction for original palette
		float i, a, t;
	
		i = 0.;
		a = i * (2.0 * M_PI / N);
		t = cos((p.x * cos(a) + p.y * sin(a)) + time * SPEED ) / 2.0;
		t = sqrt(abs(t));
		r += t + 0.5 + 0.2 * i;
		g += t + 1.0 + 0.4 * i;
		b += t + 1.5 + 0.6 * i;

		i = 1.;
		a = i * (2.0 * M_PI / N);
		t = cos((p.x * cos(a) + p.y * sin(a)) + time * SPEED ) / 2.0;
		t = sqrt(abs(t));
		r += t + 0.5 + 0.2 * i;
		g += t + 1.0 + 0.4 * i;
		b += t + 1.5 + 0.6 * i;
	
		i = 2.;
		a = i * (2.0 * M_PI / N);
		t = cos((p.x * cos(a) + p.y * sin(a)) + time * SPEED ) / 2.0;
		t = sqrt(abs(t));
		r += t + 0.5 + 0.2 * i;
		g += t + 1.0 + 0.4 * i;
		b += t + 1.5 + 0.6 * i;
	
		i = 3.;
		a = i * (2.0 * M_PI / N);
		t = cos((p.x * cos(a) + p.y * sin(a)) + time * SPEED ) / 2.0;
		t = sqrt(abs(t));
		r += t + 0.5 + 0.2 * i;
		g += t + 1.0 + 0.4 * i;
		b += t + 1.5 + 0.6 * i;
	
		i = 4.;
		a = i * (2.0 * M_PI / N);
		t = cos((p.x * cos(a) + p.y * sin(a)) + time * SPEED ) / 2.0;
		t = sqrt(abs(t));
		r += t + 0.5 + 0.2 * i;
		g += t + 1.0 + 0.4 * i;
		b += t + 1.5 + 0.6 * i;
	
		i = 5.;
		a = i * (2.0 * M_PI / N);
		t = cos((p.x * cos(a) + p.y * sin(a)) + time * SPEED ) / 2.0;
		t = sqrt(abs(t));
		r += t + 0.5 + 0.2 * i;
		g += t + 1.0 + 0.4 * i;
		b += t + 1.5 + 0.6 * i;
	
		i = 6.;
		a = i * (2.0 * M_PI / N);
		t = cos((p.x * cos(a) + p.y * sin(a)) + time * SPEED ) / 2.0;
		t = sqrt(abs(t));
		r += t + 0.5 + 0.2 * i;
		g += t + 1.0 + 0.4 * i;
		b += t + 1.5 + 0.6 * i;
	
		i = 7.;
		a = i * (2.0 * M_PI / N);
		t = cos((p.x * cos(a) + p.y * sin(a)) + time * SPEED ) / 2.0;
		t = sqrt(abs(t));
		r += t + 0.5 + 0.2 * i;
		g += t + 1.0 + 0.4 * i;
		b += t + 1.5 + 0.6 * i;
	
		i = 8.;
		a = i * (2.0 * M_PI / N);
		t = cos((p.x * cos(a) + p.y * sin(a)) + time * SPEED ) / 2.0;
		t = sqrt(abs(t));
		r += t + 0.5 + 0.2 * i;
		g += t + 1.0 + 0.4 * i;
		b += t + 1.5 + 0.6 * i;

	r = post_process(r);
	g = post_process(g);
	b = post_process(b);

	color = vec3(r,g,b);// + sun(gl_FragCoord.xy / resolution.xy);

	gl_FragColor = vec4( color, 1.0 );

}