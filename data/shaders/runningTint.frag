#version 120

uniform vec2 resolution;
uniform vec2 mouse;
uniform float time;
uniform sampler2D tex0;
uniform vec4 clrBase;
uniform vec4 clrLow;
uniform vec4 clrHigh;
uniform vec4 params;

vec4 clampColor(vec3 _clr, vec4 _low, vec4 _high) {
	_clr = clamp(mix(_clr.xyz, clrBase.xyz, clrBase.a), 0.0, 1.0);
	if((_clr.r >= _low.r && _clr.r <= _high.r) &&
		(_clr.g >= _low.g && _clr.g <= _high.g) &&
		(_clr.b >= _low.g && _clr.b <= _high.b)) {
		return vec4(_clr.xyz, _high.a);
	} else {
		return vec4(_clr.xyz, _low.a);
	}
}

const float PI = 3.1415926535;
const float TWOPI = PI*2.0;
	
vec3 hsv2rgb(float h,float s,float v) {
	return mix(vec3(1.),clamp((abs(fract(h+vec3(3.,2.,1.)/3.)*6.-3.)-1.),0.,1.),s)*v;
}

float ring( vec2 p )
{
	float len = length(p)-0.55;
	len *= length(p*p)-0.66;
  	float d = len*len*512.0;

    return 1.0-d*d;
}


vec3 ring_clr( vec2 p )
{
	float d = ring( p );
	float angle = (atan(p.x, p.y)+PI)/TWOPI;
	angle += .01 * sin(time);
	float hue = angle;
	float lum = d;
	vec3 clr = hsv2rgb(hue,0.66,lum + .1 * sin(time) );

    return clr;
}

vec2 sincostime( vec2 p ){
	p.x=p.x+sin(p.x*2.0+time)*0.4-cos(p.y*1.0-time)*0.5-sin(p.x*3.0+time)*0.3+cos(p.y*3.0-time)*0.1;
	p.y=p.y+sin(p.x*5.0+time)*0.7+cos(p.y*8.0-time)*0.3+sin(p.x*4.0+time)*0.5-cos(p.y*6.0-time)*0.3;
	return p;
}

void main(void)
{

	vec2 unipos = (gl_FragCoord.xy / resolution);
	vec2 pos = unipos*2.0-1.0;
	pos.x *= resolution.x / resolution.y;

	pos=sincostime(pos);
	vec3 clr = ring_clr(pos);
	
	gl_FragColor = clampColor(clr,clrLow,clrHigh);
	
}