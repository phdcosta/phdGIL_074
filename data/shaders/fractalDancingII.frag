/*
 * kaliset (http://www.fractalforums.com/new-theories-and-research/very-simple-formula-for-fractal-patterns/)
 * by Piers Haken.
 * 
 */

#ifdef GL_ES
precision highp float;
#endif

const float PI = 3.1415926535897932384626433832795;

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

const int max_iteration = 20;
//const float bailout = 1000000.;

float cabs (const vec2 c) { return dot(c,c); }

vec2 cconj(const vec2 c) { return vec2(c.x, -c.y); }


vec2 cmul(const vec2 c1, const vec2 c2)
{
	return vec2(
		c1.x * c2.x - c1.y * c2.y,
		c1.x * c2.y + c1.y * c2.x
	);
}

vec2 cdiv(const vec2 c1, const vec2 c2)
{
	return cmul(c1, cconj(c2)) / dot(c2, c2);
}


float saw (const float v)
{
	return sin(6.2*v) * 1.5 - 0.5;
	return abs(fract(v)-.5) * 2.5 - .5;
	return log(abs(1.-2.*fract(v+.5)) * 3.0);
}


vec3 Hue(float H)
{
	H *= 6.;
	return clamp(vec3(
		abs(H - 3.) - 1.,
		2. - abs(H - 2.),
		2. - abs(H - 4.)
	), 0., 1.);
}

vec3 HSVtoRGB(float h, float s, float v)
{
    return ((Hue(h) - 1.) * s + 1.) * v;
}

float wave(float t) { return .5 - .5 * cos (t); }

vec2 one2 = vec2(1.,0.);

vec2 circle (float a) { return vec2 (cos(a), sin(a)); }

mat2 rotate(float a)
{
	return mat2(cos(a), -sin(a), sin(a), cos(a));
}

vec2 liss (float fx, float fy)
{
	return vec2 (sin (time * fx), cos (time * fy));
}

void main( void )
{
	#if 1
	vec2 c = (mouse.xy / resolution.xy * 2. - 1.) * 1.5 - .25;
	#else
	vec2 c = .8 * vec2(wave(time/23.2347890), wave(time/37.871923));
	c = circle(time / PI) * .8 + circle(time) * .2;
	#endif
	
	//vec2 z = surfacePosition*3.;
	vec2 z = -mouse.xy / resolution.xy + 2.0 * gl_FragCoord.xy / resolution.xy; //z *= .5;
	
	#if 0
	c = z;
	z = vec2(0,0);
	#endif
	
	vec2 R = circle (time / PI / 2.0);
	
	float color = 0.;
	float m = 0.;
	for (int i = 0; i < max_iteration; ++i)
	{
		vec2 zold = z;
		
		float m = cabs(z);
		//z = abs(cmul(z, cmul(z,R)))/(m*m) + c;
		z = abs(cmul(z, R))/m + c;
		
		color += exp(-cabs(z));
		color += exp(-1./cabs(zold - z));
	}
	
	color /= float(max_iteration);

	gl_FragColor = vec4(HSVtoRGB(
		wave(color + time / 10.),
		saw(color * 3.7891),
		wave(color * 13.3234)
	), 1.0);
	
}