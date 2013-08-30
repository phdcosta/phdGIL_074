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
	vec3 total = vec3(0.0);
	
	vec4 pixcol = texture2DRect(tex0,p*size0);

//vec4 pixcol = texture2D(tex0, gl_TexCoord[0].xy);

vec4 colors[3];
colors[0] = vec4(0.,0.,1.,1.);
colors[1] = vec4(1.,1.,0.,1.);
colors[2] = vec4(1.,0.,0.,1.);
float lum = (pixcol.r+pixcol.g+pixcol.b)/3.;
int ix = (lum < 0.5)? 0:1;
vec4 thermal = mix(colors[ix],colors[ix+1],(lum-float(ix)*0.5)/0.5);

gl_FragColor = thermal;
}

