#version 120

uniform vec2 resolution;
uniform vec2 mouse;
uniform float time;
uniform sampler2D tex0;
uniform vec4 clrBase;
uniform vec4 clrLow;
uniform vec4 clrHigh;
uniform vec4 params;
uniform vec2 pos;
uniform vec2 scale;

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

#ifdef GL_ES
precision highp float;
#endif

//Object A (tunnel)
float oa(vec3 q)
{
 return cos(0.3+q.x*mouse.x/resolution.x)+cos(0.3+q.y*mouse.y/resolution.y)+cos(q.z)+cos(q.y*0.33)*.5;
}

//Scene
float o(vec3 q)
{
 return oa(q);
}

//Get Normal
vec3 gn(vec3 q)
{
 vec3 f=vec3(.01,0,0);
 return normalize(vec3(o(q+f.xyy),o(q+f.yxy),o(q+f.yyx)));
}

//MainLoop
void main(void)
{
 vec2 p = -1.0 + 2.0 * gl_FragCoord.xy / resolution.xy;
 p.x *= resolution.x/resolution.y;
	//p -= mouse;
 
 vec4 c=vec4(1.0);
 vec3 org=vec3(sin(time)*.5,cos(time*.5)*.25+.25,time),dir=normalize(vec3(p.x*1.6,p.y,1.0)),q=org,pp;
 float d=.0;

 //First raymarching
 for(int i=0;i<64;i++)
 {
  d=o(q);
  q+=d*dir;
 }
 pp=q;
 float f=length(q-org)*0.02;

 //Second raymarching (reflection)
 dir=reflect(dir,gn(q));
 q+=dir;
 for(int i=0;i<64;i++)
 {
 d=o(q);
 q+=d*dir;
 }
 c=max(dot(gn(q),vec3(.1,.1,.0)),.0)+vec4(.3,cos(time*.5)*.5+.6,sin(time*.5)*.5+.3,1.)*min(length(q-org)*.01,1.);


 //Final Color
 vec4 fcolor = ((c+vec4(f))+(0.9-min(pp.y+2.0,1.0))*-vec4(0.1,.2,.8,1.))*min(time*.5,1.);
 gl_FragColor = clampColor(fcolor.xyz,clrLow,clrHigh);
}
