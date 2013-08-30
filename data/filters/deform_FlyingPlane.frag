#version 120

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

vec2 zoom0 = vec2(12., 2.);

void main( void ) {

	vec2 p = -1.0 + 2.0 * gl_TexCoord[0].st / size0;
	vec2 uv;

	float an = 3.1415927410125*1.;
	float seno = sin(an);
	float cose = cos(an);

	float x = p.x*cose-p.y*seno;
	float y = p.x*seno+p.y*cose;

	uv.x = abs(1.0-fract(0.25*x/abs(y) + 0.5));
	uv.y = abs(1.0-fract(.25*time + 0.5/abs(y)));

	gl_FragColor = vec4(texture2DRect(tex0,uv*size0).xyz * y*y, 1.0);
}

