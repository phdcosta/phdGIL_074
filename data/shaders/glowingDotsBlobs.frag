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

// By @paulofalcao
//
// Blobs changed by @hintz

#ifdef GL_ES
precision highp float;
#endif

float makePoint(float x,float y,float fx,float fy,float sx,float sy,float t)
{
   float xx=x+sin(t*fx)*cos(t*sx)*(2.-mouse.y/resolution.y*2.);
   float yy=y+cos(t*fy)*sin(t*sy)*(2.-mouse.y/resolution.y*2.);
  
   return 0.4/sqrt((xx*xx+yy*yy));
}

void main() 
{
   vec2 p=(gl_FragCoord.xy/resolution.x)*2.0-vec2(1.0,resolution.y/resolution.x);
   p = gl_FragCoord.xy / resolution.xy - mouse.xy / resolution.xy;
   p=p*3.0;
   
   float x=p.x;
   float y=p.y*-1.0;

   float a=makePoint(x,y,3.3,2.9,1.3,0.3,time*.51);
   a+=makePoint(x,y,1.9,2.0,0.4,0.4,time*.27);
   a+=makePoint(x,y,0.2,0.7,0.4,0.5,time*.35);
   a+=makePoint(x,y,2.3,0.1,0.6,1.3,time*.72);
   a+=makePoint(x,y,0.8,1.7,0.5,0.4,time*.17);
   a+=makePoint(x,y,0.3,1.0,0.4,0.4,time*.23);
   a+=makePoint(x,y,1.4,1.3,0.4,1.5,time*.31);
   a+=makePoint(x,y,1.3,2.1,0.6,0.3,time*.47);
   a+=makePoint(x,y,1.8,1.4,0.5,1.4,time*.13);   
   
   float b=makePoint(x,y,1.2,1.9,0.3,0.3,time*.13);
   b+=makePoint(x,y,0.7,2.7,0.4,4.0,time*.17);
   b+=makePoint(x,y,1.4,0.6,0.4,0.5,time*.23);
   b+=makePoint(x,y,2.6,0.4,0.6,0.3,time*.31);
   b+=makePoint(x,y,0.1,1.4,0.5,0.4,time*.41);
   b+=makePoint(x,y,0.7,1.7,0.4,0.4,time*.29);
   b+=makePoint(x,y,0.8,0.5,0.4,0.5,time*.07);
   b+=makePoint(x,y,1.4,0.9,0.6,0.3,time*.11);
   b+=makePoint(x,y,0.7,1.3,0.5,0.4,time*.61);

   float c=makePoint(x,y,3.7,0.3,0.3,0.3,time*.39);
   c+=makePoint(x,y,1.9,1.3,0.4,0.4,time*.61);
   c+=makePoint(x,y,0.8,0.9,0.4,0.5,time*.51);
   c+=makePoint(x,y,1.2,1.7,0.6,0.3,time*.31);
   c+=makePoint(x,y,0.3,0.6,0.5,0.4,time*.23);
   c+=makePoint(x,y,0.3,0.3,0.4,0.4,time*.43);
   c+=makePoint(x,y,1.4,0.8,0.4,0.5,time*.11);
   c+=makePoint(x,y,0.2,0.6,0.6,0.3,time*.29);
   c+=makePoint(x,y,1.3,0.5,0.5,0.4,time*.07);
   
   vec3 d=vec3(b*c,a*c,a*b)/200.0;
   
   float ph = (d.x+d.y+d.z)/6.0;
	
   gl_FragColor = vec4(d.x,d.y,d.z,ph);
}