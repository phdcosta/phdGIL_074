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

	vec2 p = -1.0 + 2.0 * gl_TexCoord[0].st / size0 + vec2(.5,.5);
	vec2 uv;

	float a = atan(p.y,p.x) + 0.5*time;
	float r = sqrt(dot(p,p));

	uv.x = abs(1.0-fract(.25*time+.1/r*zoom0.x));
	uv.y = abs(fract(a/3.1415927410125*zoom0.y));

	vec3 col =  texture2DRect(tex0,uv*size0).xyz;

	gl_FragColor = vec4(col*r,1.0);
}

