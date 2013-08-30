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

const float pi=3.1415926535;

vec2 clog(vec2 v)
{
	return vec2(0.5*log(v.x*v.x+v.y*v.y),atan(-v.y,v.x));
}

vec2 cdiv(vec2 a,vec2 b)
{
	return vec2(a.x*b.x+a.y*b.y,a.y*b.x-a.x*b.y)/(b.x*b.x+b.y*b.y);
}

float l(float i)
{
	return pow(sin(time*i)*0.5+0.5,1.0);
}

vec4 checkerboard(vec2 pos)
{
	float a=(fract(pos.x)-0.5)*(fract(pos.y)-0.5);
	float b=(0.5-fract(pos.x))*(0.5-fract(pos.y));
	vec4 xx = vec4(abs(a+b)*2.0,-abs(b+a)*2.0,(sin(time*3.0)*0.1+0.2),1.0);
	vec4 yy = vec4(-abs(a+b)*2.0,abs(b+a)*2.0,(sin(time*3.0)*0.1+0.2),1.0);
	vec4 zz = vec4(0.3*(sin(time*12.0+(pos.y)*10.4)*0.5+0.5)+0.3*(sin(time*9.0+(pos.x)*10.4)*0.5+0.5),0.5*(sin(time*14.0+(pos.y)*10.4)*0.5+0.5),0.9*(sin(time*7.0+(pos.x)*14.3)*0.5+0.5),1.0);
	float c1=fract(((pos.x+1.0)*5.0));
	float c2=fract(((pos.y+1.0)*5.0));
	vec4 tt = xx;
	if(c1 > 0.5 &&  c2 > 0.5 || c1 < 0.5 && c2 < 0.5) tt = yy;
	float t = (l(1.0)*l(3.0)*(-l(2.0))*(-l(4.75))*10.0+0.4);
	if(a>0.05*t|| b < -0.05*t) return tt;
	else return zz;
}

vec4 grid(vec2 pos,float thickness)
{
	vec2 a=abs(fract(pos)-0.5);
	if(a.x>thickness && a.y>thickness) return vec4(0.7,0.72,0.7,1.0);
	else return vec4(0.3,0.33,0.3,1.0);
}

void main()
{
	vec2 position=2.0*(gl_FragCoord.xy/resolution.xy)-1.0;

	const float p1=1.0;
	const float p2=1.0;
	float u_corner=2.0*pi*p2;
	float v_corner=log(256.0)*p1;
	float diag=sqrt(u_corner*u_corner+v_corner*v_corner);
	float sin_a=v_corner/diag;
	float cos_a=u_corner/diag;
	float scale=diag/2.0/pi;

	vec2 offset=vec2(sin(time*1.5), sin(time*0.4));
	vec2 p=clog(cdiv(vec2(1.0,0.0),position+offset))-clog(cdiv(vec2(1.0,0.0),position-offset));

	vec2 rotated=vec2(p.x*cos_a-p.y*sin_a,
		          p.x*sin_a+p.y*cos_a);
	vec2 scaled=rotated*scale/vec2(log(256.0),2.0*pi);
//	vec2 scaled=p/vec2(log(256.0),2.0*pi);
//	vec2 translated=scaled-vec2(0.35+time*0.1,0.0);
	vec2 translated=scaled-vec2(time*0.5,0.0);

	vec4 color = checkerboard(translated*6.0);
	
	gl_FragColor = clampColor(color.rgb, clrLow, clrHigh);
// 		gl_FragColor=grid(translated*8.0+0.3,0.1);
}