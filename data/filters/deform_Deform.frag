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
	vec2 m = -1.0 + 2.0 * mouse.xy / resolution.xy;
	float a1 = atan(p.y-m.y,p.x-m.x);
	float r1 = sqrt(dot(p-m,p-m));
	float a2 = atan(p.y+m.y,p.x+m.x);
	float r2 = sqrt(dot(p+m,p+m));
	vec2 uv;
	uv.x = abs(fract(0.2*time + (r1-r2)*0.25));
	uv.y = abs(fract(sin(2.0*(a1-a2))));
	float w = r1*r2*0.8;
	vec3 col = texture2DRect(tex0,uv*size0).xyz;
	gl_FragColor = vec4(col/(.1+w),1.0);
}
