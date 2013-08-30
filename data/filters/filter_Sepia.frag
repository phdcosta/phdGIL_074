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

	vec2 p = gl_TexCoord[0].st / size0 + vec2(.0,.0);

	vec3 col = texture2DRect(tex0, p*size0).bgr;

	float y = 0.3 *col.r + 0.59 * col.g + 0.11 * col.b;

	gl_FragColor = vec4(y + 0.15, y + 0.07, y - 0.12, 1.0);
}

