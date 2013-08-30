#version 120

#extension GL_ARB_texture_rectangle : enable

#ifdef GL_ES
precision highp float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
uniform sampler2DRect tex0;
uniform vec2 size0;
uniform sampler2DRect tex1;
uniform vec2 size1;
uniform float alpha;

vec2 zoom0 = vec2(2., 2.);
float pi = 3.1415927410125;

void main( void ) {
	vec2 p = -1.0 + 2.0 * gl_TexCoord[0].st / size0 + vec2(.0,.0);
	vec2 uv;
	float a = atan(p.y,p.x);
	float r = sqrt(dot(p,p));
	uv.x = abs(fract((cos(0.6+time) + cos(cos(1.2+time)+a)/r)/zoom0.x));
	uv.y = abs(fract((cos(0.3+time) + sin(cos(2.0+time)+a)/r)/zoom0.y));
	vec3 col =  texture2DRect(tex0,uv*.25*size0).xyz;
	gl_FragColor = vec4(col*r*r,1.0);
}
