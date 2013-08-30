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

vec2 zoom0 = vec2(.5,.5);

float pi = 3.1415927410125;

vec3 hsv2rgb(float h,float s,float v) {
	return mix(vec3(1.),clamp((abs(fract(h+vec3(3.,2.,1.)/3.)*6.-3.)-1.),0.,1.),s)*v;
}

void main( void ) {

	vec2 p = -1.0 + 2.0 * gl_TexCoord[0].st / size0;

	vec2 cst = vec2(cos(.5*time), sin(.5*time) );
	mat2 rot = mat2(cst.x,-cst.y,cst.y,cst.x);

	vec2 r2 = rot * p * zoom0;
	r2.x = abs(fract(r2.x));
	r2.y = abs(fract(r2.y));

	p.x = abs(fract(p.x+time*.1*zoom0.x));
	p.y = abs(fract(p.y+time*.1*zoom0.y));

	vec4 col1 = texture2DRect(tex0,r2*size0) * alpha;
	vec4 col2 = texture2DRect(tex1,p*size1) * (1.0 - alpha);

	gl_FragColor = col2 + col1;

}

