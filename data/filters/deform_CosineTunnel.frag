#version 120

#ifdef GL_ES
precision highp float;
#endif

uniform float alpha;

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

uniform vec4 params;

uniform sampler2DRect tex0;
uniform vec2 size0;

uniform sampler2DRect tex1;
uniform vec2 size1;

vec2 zoom0 = vec2(1., 1.);

void main( void ) {
	vec2 p = -1.0 + 2.0 * gl_TexCoord[0].st / size0 + vec2(.0,.0);
	vec2 uv;

	float r = sqrt( dot(p,p) );
	float a = atan(p.y,p.x) + 0.5*sin(0.5*r+0.5*time);

	float sides = (params[0] * 16.0) - fract(params[0] * 16.0);

	float s = 0.5 + 0.75*cos(sides*a);

	s = smoothstep(0.0,1.0,s);
	s = smoothstep(0.0,1.0,s);
	s = smoothstep(0.0,1.0,s);
	s = smoothstep(0.0,1.0,s);

	uv.x = abs(fract((-.5*time + 1.0/( r + .2*s))*zoom0.x));
	uv.y = abs(fract((3.0*a/3.1415927410125)*zoom0.x));

	float w = (0.5 + 0.5*s)*r*r;
	vec3 col = texture2DRect(tex0,uv*size0).xyz;

	float ao = 0.5 + 0.5*cos(4.0*a);
	ao = smoothstep(0.0,0.4,ao)-smoothstep(0.4,0.7,ao);
	ao = 1.0-0.5*ao*r;

	gl_FragColor = vec4(col*w*ao*1.5, 1.0);
}
