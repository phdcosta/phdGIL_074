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

void rgb2hsl(vec3 rgb, out float h, out float s, float l)
{
    float maxval = max(rgb.r , max(rgb.g, rgb.b));
    float minval = min(rgb.r, min(rgb.g, rgb.b));
    float delta = maxval - minval;
 
    l = (minval + maxval) / 2.0;
    s = 0.0;
    if (l > 0.0 && l < 1.0)
        s = delta / (l < 0.5 ? 2.0 * l : 2.0 - 2.0 * l);
    h = 0.0;
    if (delta > 0.0)
    {
        if (rgb.r == maxval && rgb.g != maxval)
            h += (rgb.g - rgb.b ) / delta;
        if (rgb.g == maxval && rgb.b != maxval)
            h += 2.0  + (rgb.b - rgb.r) / delta;
        if (rgb.b == maxval && rgb.r != maxval)
            h += 4.0 + (rgb.r - rgb.g) / delta;
        h *= 60.0;
    }
}

void main( void ) {

	vec2 p = gl_TexCoord[0].st;

	vec3 col = texture2DRect(tex0, p).rgb;

	float h, s, l;

	rgb2hsl(col, h, s, l);

	float y = 0.3 * col.r + 0.59 * col.g + 0.11 * col.b;

	vec3 result;
	if (h <= 5.0 || h >= 355.0) // red
	//if (h >= 60.0 && h <= 180.0) // green
	//if (h >= 180.0 && h <= 260.0) // blue
	//if (h >= 30.0 && h <= 60.0) // yellow
		result = col;
	else
		result = vec3(y, y, y);

	gl_FragColor.a = 1.0;
	gl_FragColor.rgb = result;
}

