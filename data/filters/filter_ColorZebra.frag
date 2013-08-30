//COLOR ZEBRA
//https://www.shadertoy.com/view/Xsl3z8

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

void main(void)
{
	float phase=sin(time*.25);
	float levels=8.;
	//vec2 xy = gl_FragCoord.xy / iResolution.xy;
	vec4 tx = texture2DRect(tex0, gl_FragCoord.xy);
	
	//float x = (pix.r + pix.g + pix.b) / 3.0;//use this for BW effect.
	vec4 x=tx;
	
	x = mod(x + phase, 1.);
	x = floor(x*levels);
	x = mod(x,2.);
	
	gl_FragColor= vec4(vec3(x), tx.a);
}