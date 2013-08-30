#version 120

uniform float time;
uniform float alpha;
uniform vec2 mouse;
uniform vec2 resolution;
uniform vec4 clrBase;
uniform vec4 clrLow;
uniform vec4 clrHigh;
uniform vec4 params;
uniform vec4 fftA;
uniform vec4 fftB;
uniform vec2 pos;
uniform vec2 scale;
uniform sampler2DRect tex0;

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

float makePoint(float x,float y,float fx,float fy,float sx,float sy,float t) {
   float xx=x+sin(t*fx)*cos(t*sx)/mouse.x*resolution.x*.2;
   float yy=y+cos(t*fy)*sin(t*sy)/mouse.y*resolution.y*.2;
   return .4/sqrt(abs(xx*xx+yy*yy));
}

void main() {
   vec2 p=(gl_FragCoord.xy/resolution.x)*2.0-vec2(1.0,resolution.y/resolution.x);
   p=p*3.0;
   p = gl_FragCoord.xy / resolution.xy - mouse.xy / resolution.xy;
   p=p*5.0;
   float x=p.x;
   float y=p.y;
   float a=makePoint(x,y,3.3,2.9,1.3,0.3,time);
   a+=makePoint(x,y,1.9,2.0,0.4,0.4,time);
   a+=makePoint(x,y,0.2,0.7,0.4,0.5,time);
  
   float b=makePoint(x,y,1.2,1.9,0.3,0.3,time);
   b+=makePoint(x,y,0.7,2.7,0.4,4.0,time);
   b+=makePoint(x,y,1.4,0.6,0.4,0.5,time);
   b+=makePoint(x,y,2.6,0.4,0.6,0.3,time);
   b+=makePoint(x,y,0.1,1.4,0.5,0.4,time);
   b+=makePoint(x,y,0.7,1.7,0.4,0.4,time);
   b+=makePoint(x,y,0.8,0.5,0.4,0.5,time);
   b+=makePoint(x,y,1.4,0.9,0.6,0.3,time);
   b+=makePoint(x,y,0.7,1.3,0.5,0.4,time);
  
   float c=makePoint(x,y,3.7,0.3,0.3,0.3,time);
   c+=makePoint(x,y,1.9,1.3,0.4,0.4,time);
   c+=makePoint(x,y,0.8,0.9,0.4,0.5,time);
   c+=makePoint(x,y,1.2,1.7,0.6,0.3,time);
   c+=makePoint(x,y,0.3,0.6,0.5,0.4,time);
   c+=makePoint(x,y,0.3,0.3,0.4,0.4,time);
   c+=makePoint(x,y,1.4,0.8,0.4,0.5,time);
   c+=makePoint(x,y,0.2,0.6,0.6,0.3,time);
   c+=makePoint(x,y,1.3,0.5,0.5,0.4,time);
   vec3 d=vec3(b*c,a*c,a*b)/100.0;
   gl_FragColor = clampColor(vec3(d.x,d.y,d.z), clrLow, clrHigh);
}