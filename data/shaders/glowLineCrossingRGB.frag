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

// the.savage@hotmail.co.uk

#ifdef GL_ES
precision mediump float;
#endif

void main(void)
{
	vec2 vPixel=gl_FragCoord.xy;

	vec2 vScreen=resolution;
	vec2 vMouse=vec2(0.1,0.1)+mouse/resolution;

	float fTime=time*0.5;

	vec3 v=vec3(
		vPixel.x+vPixel.y+cos(sin(fTime)*2.0)*100.0+sin(vPixel.x/100.0)*100.0,
		vPixel.y/vScreen.y/(vMouse.x*5.0)+fTime,
		vPixel.x/vScreen.x/(vMouse.y*5.0));
	
	float r=abs(sin(v.y+fTime)/2.0+v.z/2.0-v.y-v.z+fTime);
	float g=abs(sin(r+sin(v.x/1000.0+fTime)+sin(vPixel.y/100.0+fTime)+sin((vPixel.x+vPixel.y)/100.0)*3.0));
	float b=abs(sin(g+cos(v.y+v.z+g)+cos(v.z)+sin(v.x/1000.0)));

	float n=vMouse.x*50.0;
	vec2 p=-(n*0.5)+n*vPixel/vScreen;

	float an=fTime*2.15;

	float x=p.x*cos(an)-p.y*sin(an);
	float y=p.x*sin(an)+p.y*cos(an);

	gl_FragColor= clampColor(vec3(1.0-(vec3(r,g,b)*x*x)),clrLow,clrHigh);
}
