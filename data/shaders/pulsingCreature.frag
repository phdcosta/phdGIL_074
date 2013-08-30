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

float u( float x ) { return (x>0.0)?1.0:x; }

void main(void) {
	float td8 = time / 1.0;
	vec2 p = gl_FragCoord.xy/resolution.xy-mouse.xy/resolution.xy;
	float a = atan(p.x,p.y);
	a = a*a;
	float r = length(p)*3.5;
	float sides = 3.2;
	float rings = .5;
	float w = cos(3.1415927*td8-r);
	float h = 0.75+0.5*cos(sides*a-w*sides*rings+r*3.0);
	float d = 0.5+0.75;
	d*=pow(h,1.0*r)*(0.75+0.1*w);
	float col = u( d-r ) * sqrt(1.0-r/d)*r*1.66;
	col *= 1.25+0.25*cos((12.0*a-w*7.0+r*8.0)/2.0);
	col *= 1.0 - 0.35*(0.5+0.5*sin(r*30.0))*(0.5+0.5*cos(12.0*a-w*7.0+r*8.0));
	gl_FragColor = clampColor(vec3(col,col-h*0.5+r*.2 + 0.155*h*(1.0-r),col-h*r + 0.1*h*(1.0-r)),clrLow,clrHigh);
}
