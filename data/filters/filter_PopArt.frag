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

vec3 deform( in vec2 p, float scale ) {

	vec2 uv;

	float mtime = scale+time;
	float a = atan(p.y,p.x);
	float r = sqrt(dot(p,p));
	float s = r * (1.0+0.5*cos(mtime*1.7));

	uv.x = abs(fract((.1*mtime +.05*p.y+.05*cos(-mtime+a*3.0)/s)/zoom0.x));
	uv.y = abs(fract((.1*mtime +.05*p.x+.05*sin(-mtime+a*3.0)/s)/zoom0.y));

	float w = 0.8-0.2*cos(mtime+3.0*a);

	vec3 res = texture2DRect(tex0,uv*size0).xyz*w;

	return  res*res;
}

void main( void ) {

	vec2 p = gl_TexCoord[0].st / size0 + vec2(.0,.0);

	vec3 col = texture2DRect(tex0, p*size0).bgr;

	float y = 0.3 *col.r + 0.59 * col.g + 0.11 * col.b;

	y = y < 0.3 ? 0.0 : (y < 0.6 ? 0.5 : 1.0);

	if (y == 0.5)
		col = vec3(0.8, 0.0, 0.0);
	else if (y == 1.0)
		col = vec3(0.9, 0.9, 0.0);
	else
		col = vec3(0.0, 0.0, 0.0);
        
	gl_FragColor.a = 1.0;
	gl_FragColor.rgb = col;
}

