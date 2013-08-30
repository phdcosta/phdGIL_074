//Metamaterial
//https://www.shadertoy.com/view/lssGz8

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

//utility
float remap(float value, float inputMin, float inputMax, float outputMin, float outputMax)
{
    return (value - inputMin) * ((outputMax - outputMin) / (inputMax - inputMin)) + outputMin;
}
float rand(vec2 n, float time)
{
  return 0.5 + 0.5 * 
     fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453 + time);
}

struct Circle
{
	vec2 center;
	float radius;
};
	
vec4 circle_mask_color(Circle circle, vec2 position)
{
	float d = distance(circle.center, position);
	if(d > circle.radius)
	{
		return vec4(0.0, 0.0, 0.0, 1.0);
	}
	
	float distanceFromCircle = circle.radius - d;
	float intencity = smoothstep(
								    0.0, 1.0, 
								    clamp(
									    remap(distanceFromCircle, 0.0, 0.1, 0.0, 1.0),
									    0.0,
									    1.0
								    )
								);
	return vec4(intencity, intencity, intencity, 1.0);
}

vec4 mask_blend(vec4 a, vec4 b)
{
	vec4 one = vec4(1.0, 1.0, 1.0, 1.0);
	return one - (one - a) * (one - b);
}

float f1(float x)
{
	return -4.0 * pow(x - 0.5, 2.0) + 1.0;
}
	
void main(void)
{
	vec2 uv = gl_FragCoord.xy / size0.xy;
	
	float wide = size0.x / size0.y;
	float high = 1.0;
	
	vec2 position = vec2(uv.x * wide, uv.y);
	
	Circle circle_a = Circle(vec2(0.5, 0.5), 0.5);
	Circle circle_b = Circle(vec2(wide - 0.5, 0.5), 0.5);
	vec4 mask_a = circle_mask_color(circle_a, position);
	vec4 mask_b = circle_mask_color(circle_b, position);
	vec4 mask = mask_blend(mask_a, mask_b);
	
	float greenness = 0.4;
	vec4 coloring = vec4(1.0 - greenness, 1.0, 1.0 - greenness, 1.0);
	
	float noise = rand(uv * vec2(0.1, 1.0), time * 5.0);
	float noiseColor = 1.0 - (1.0 - noise) * 0.3;
	vec4 noising = vec4(noiseColor, noiseColor, noiseColor, 1.0);
	
	float warpLine = fract(-time * 0.5);
	
	/** debug
	if(abs(uv.y - warpLine) < 0.003)
	{
		gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
		return;
	}
    */
	
	float warpLen = 0.1;
	float warpArg01 = remap(clamp((position.y - warpLine) - warpLen * 0.5, 0.0, warpLen), 0.0, warpLen, 0.0, 1.0);
	float offset = sin(warpArg01 * 10.0)  * f1(warpArg01);
	
	offset = 0.0; // turn warp line off

	vec4 lineNoise = vec4(1.0, 1.0, 1.0, 1.0);
	if(abs(uv.y - fract(-time * 27.0)) < 0.0005)
	{
		lineNoise = vec4(0.5, 0.5, 0.5, 1.0);
	}
	
	vec4 base = texture2DRect(tex0, (uv + vec2(offset * 0.02, 0.0)) * size0);
	gl_FragColor = base * mask * coloring * noising * lineNoise;

}