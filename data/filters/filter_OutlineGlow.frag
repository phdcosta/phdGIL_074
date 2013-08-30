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

	vec2 p = gl_TexCoord[0].st;

	vec3 t1 = texture2DRect(tex0, vec2(p.x - 1.0, p.y - 1.0)).bgr;
	vec3 t2 = texture2DRect(tex0, vec2(p.x, p.y - 1.0)).bgr;
	vec3 t3 = texture2DRect(tex0, vec2(p.x + 1.0, p.y - 1.0)).bgr;
	vec3 t4 = texture2DRect(tex0, vec2(p.x - 1.0, p.y)).bgr;
	vec3 t5 = texture2DRect(tex0, p).bgr;
	vec3 t6 = texture2DRect(tex0, vec2(p.x + 1.0, p.y)).bgr;
	vec3 t7 = texture2DRect(tex0, vec2(p.x - 1.0, p.y + 1.0)).bgr;
	vec3 t8 = texture2DRect(tex0, vec2(p.x, p.y + 1.0)).bgr;
	vec3 t9 = texture2DRect(tex0, vec2(p.x + 1.0, p.y + 1.0)).bgr;
   
	vec3 xx= t1 + 2.0*t2 + t3 - t7 - 2.0*t8 - t9;
	vec3 yy = t1 - t3 + 2.0*t4 - 2.0*t6 + t7 - t9;
    
	vec3 rr = sqrt(xx * xx + yy * yy);
    
	gl_FragColor.a = 1.0;
	gl_FragColor.rgb = rr * 2.0 * t5;
}

