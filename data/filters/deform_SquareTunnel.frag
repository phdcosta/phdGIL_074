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

vec2 zoom0 = vec2(2., 2.);

void main( void ) {

	vec2 p = -1.0 + 2.0 * gl_TexCoord[0].st / size0 + vec2(.0,.0);
	float r = pow( pow(p.x*p.x,16.0) + pow(p.y*p.y,16.0), 1.0/32.0 );

	vec2 uv;

	uv.x = .25*time + 0.5/r;
	uv.y = atan(p.y,p.x)/3.1415927410125;
	uv.x = abs(fract(uv.x * zoom0.x));
	uv.y = abs(fract(uv.y * zoom0.y + 0.5));

	//float ay = mouse.y / resolution.y;

	vec3 col = texture2DRect(tex0,uv*size0).xyz;

	gl_FragColor = vec4(col*r*r, 1.0);
}

