
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
	uv.x = abs(fract((7.0*a/pi)/zoom0.x));
	uv.y = abs(fract((-time+ sin(7.0*r+time) + .7*cos(time+7.0*a))/zoom0.y));
	float w = .5+.5*(sin(time+7.0*r)+ .7*cos(time+7.0*a));
	vec3 col =  texture2DRect(tex0,uv*.5*size0).xyz;
	gl_FragColor = vec4(col*w,1.0);
}