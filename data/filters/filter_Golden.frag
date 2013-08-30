//Mod Filter
//https://www.shadertoy.com/view/4sfGR4

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
    vec2 uv = gl_FragCoord.xy / size0.xy;
	
	/*
	float spinT = time*0.5;
	vec2 cutDir = vec2(cos(spinT), sin(spinT));
	if( dot(cutDir, uv-vec2(0.5,0.5)) < 0.0 )
	{
		gl_FragColor = vec4( texture2DRect( tex0, uv*size0 ).rgb, 1.0 );
		return;
	}
	*/

	vec2 step = 1.0 / size0.xy;
	
	vec3 texA = texture2DRect( tex0, (uv + vec2(-step.x, -step.y) * 1.5) * size0 ).rgb;
	vec3 texB = texture2DRect( tex0, (uv + vec2( step.x, -step.y) * 1.5) * size0 ).rgb;
	vec3 texC = texture2DRect( tex0, (uv + vec2(-step.x,  step.y) * 1.5) * size0 ).rgb;
	vec3 texD = texture2DRect( tex0, (uv + vec2( step.x,  step.y) * 1.5) * size0 ).rgb;
	
	float shadeA = dot(texA, vec3(0.333333));
	float shadeB = dot(texB, vec3(0.333333));
	float shadeC = dot(texC, vec3(0.333333));
	float shadeD = dot(texD, vec3(0.333333));

	float shade = 15.0 * pow(max(abs(shadeA - shadeD), abs(shadeB - shadeC)), 0.5);
	
	vec3 col = mix(vec3(0.1, 0.18, 0.3), vec3(0.4, 0.3, 0.2), shade);
	
    gl_FragColor = vec4(col,1.0);
}