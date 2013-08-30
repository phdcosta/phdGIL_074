// PULSE

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
	vec2 halfres = resolution.xy/2.0;
	vec2 cPos = gl_FragCoord.xy;
	cPos.x -= 0.5*halfres.x*sin(time/2.0)+0.3*halfres.x*cos(time)+halfres.x;
	cPos.y -= 0.4*halfres.y*sin(time/5.0)+0.3*halfres.y*cos(time)+halfres.y;
	float cLength = length(cPos);
	vec2 uv = gl_FragCoord.xy/resolution.xy+(cPos/cLength)*sin(cLength/30.0-time*10.0)/25.0;
	uv = vec2(abs(fract((uv.x)/zoom0.x)), abs(fract((uv.y))/zoom0.y));
	vec3 col = texture2DRect(tex0,uv*size0).xyz*50.0/cLength;
	gl_FragColor = vec4(col,1.0);
}