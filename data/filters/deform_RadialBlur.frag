#version 120

#extension GL_ARB_texture_rectangle : enable

#ifdef GL_ES
precision highp float;
#endif

uniform float alpha;

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

uniform sampler2DRect tex0;
uniform vec2 size0;

uniform sampler2DRect tex1;
uniform vec2 size1;

vec2 zoom0 = vec2(2., 2.);

float pi = 3.1415927410125;

vec3 deform( in vec2 p ) {
	vec2 uv;
	vec2 q = vec2( sin(1.1*time+p.x),sin(1.2*time+p.y) );
	float a = atan(q.y,q.x);
	float r = sqrt(dot(q,q));
	uv.x = abs(fract((sin(0.0+1.0*time)+p.x*sqrt(r*r+1.0))/zoom0.x));
	uv.y = abs(fract((sin(0.6+1.1*time)+p.y*sqrt(r*r+1.0))/zoom0.y));
	return texture2DRect(tex0,uv*.5*size0).xyz;
}

void main( void ) {
	vec2 p = -1.0 + 2.0 * gl_TexCoord[0].st / size0 + vec2(.0,.0);
	vec2 s = p;
	vec3 total = vec3(0.0);
	vec2 d = (vec2(0.0,0.0)-p)/40.0;

	float w = 1.0;
	for( int i=0; i<10; i++ ) {
		vec3 res = deform(s);
		res = smoothstep(0.1,1.0,res*res);
		total += w*res;
		w *= .99;
		s += d;
	}
	total /= 10.0;

	float r = 1.5/(1.0+dot(p,p));

	gl_FragColor = vec4( total*r,1.0);
}

