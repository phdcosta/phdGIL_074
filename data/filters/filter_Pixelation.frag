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

void main( void ) {

	vec2 p = gl_TexCoord[0].st / size0;

	float dx = 9.*(1./size0.x);
	float dy = 6.*(1./size0.y);

	vec2 coord = vec2(dx*floor(p.x/dx), dy*floor(p.y/dy));

	gl_FragColor = texture2DRect(tex0, coord*size0);
}

