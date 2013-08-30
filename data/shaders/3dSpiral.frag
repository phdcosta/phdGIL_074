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

#define R(p,a) p=vec2(p.x*cos(a)+p.y*sin(a),p.y*cos(a)-p.x*sin(a));

//Simple raymarching sandbox with camera

//Raymarching Distance Fields
//About http://www.iquilezles.org/www/articles/raymarchingdf/raymarchingdf.htm
//Also known as Sphere Tracing

// ray rotation along path by @psonice

//Util Start
vec2 ObjUnion(in vec2 obj0,in vec2 obj1){
  if (obj0.x<obj1.x)
  	return obj0;
  else
  	return obj1;
}
//Util End

//Scene Start

//Floor
vec2 obj0(in vec3 p){
  return vec2(p.y+3.0,0);
}
//Floor Color (checkerboard)
vec3 obj0_c(in vec3 p){
 if (fract(p.x*.5)>.5)
   if (fract(p.z*.5)>.5)
     return vec3(0,0,0);
   else
     return vec3(1,1,1);
 else
   if (fract(p.z*.5)>.5)
     return vec3(1,1,1);
   else
     	return vec3(0,0,0);
}

//IQs RoundBox (try other objects http://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm)
vec2 obj1(in vec3 p){
  return vec2(length(max(abs(p)-vec3(1,.125,1),0.0))-0.25,1);
}

//RoundBox with simple solid color
vec3 obj1_c(in vec3 p){
	return vec3(1.0,0.5,0.2);
}

// SNIPPP
// ------
vec2 f(vec3 p) {
	vec3 p0 = p;
	vec3 p1 = p;
	
	p0.xz = mod(p0.xz,4.) - 2.;
	p1.xz = mod(p1.xz+2.,4.) - 2.;
	p1.y += 0.3;
	
	float d0 = obj1(p0).x;
	float d1 = obj1(p1).x;

	float d = min(d0, d1);
	
	return vec2(d, 1.);
}
// ------

//Objects union
vec2 inObj(in vec3 p){
  return f(p);//ObjUnion(obj0(p),obj1(p));
}

//Scene End

void main(void){
  vec2 vPos=gl_FragCoord.xy/resolution.xy;
    vPos -= (mouse.xy/ resolution.xy);

  //Camera animation
  vec3 vuv=vec3(0,1,0);//Change camere up vector here
  vec3 vrp=vec3(0,4,0.); //Change camere view here
  vec3 prp=vec3(-sin(time)*8.0,4,cos(time)*8.0); //Change camera path position here
  prp = vec3(2., 4.,time*4.);
	vec3 oPrp = prp;
  //Camera setup
  vec3 vpn=normalize(vrp-prp);
  vec3 u=normalize(cross(vuv,vpn));
  vec3 v=cross(vpn,u);
  vec3 vcv=(prp+vpn);
  vec3 scrCoord=vcv+vPos.x*u*resolution.x/resolution.y+vPos.y*v;
  vec3 scp=normalize(scrCoord-prp);

  //Raymarching
  const vec3 e=vec3(0.1,0,0);
  const float maxd=240.0; //Max depth

  vec2 s=vec2(0.1,0.0);
  vec3 c,p,n;

  float f=1.0;
  for(int i=0;i<256;i++){
    if (abs(s.x)<.01||f>maxd) break;
    f+=s.x;
	  //scp.y -= 4.;
	  //scp.y += 4.;
    p=prp+scp*f*.25;
	  p.y -= 4.;
    p.xy = R(p.xy, p.z*0.5);
	  p.y += 4.;
	 
    s=inObj(p);
	  //prp.xy = R(prp.xy, prp.z*0.1);
  }
  
  if (f<maxd){
    if (s.y==0.0)
      c=obj0_c(p);
    else
      c=obj1_c(p);
    n=normalize(
      vec3(s.x-inObj(p-e.xyy).x,
           s.x-inObj(p-e.yxy).x,
           s.x-inObj(p-e.yyx).x));
	float b=dot(n,normalize(prp-p));
	vec3 color = (b*c+pow(b,8.0))*(1.0-f*.001);
	gl_FragColor = clampColor(color, clrLow, clrHigh);//simple phong LightPosition=CameraPosition
  }
  else gl_FragColor = clampColor(vec3(0,0,0),clrLow,clrHigh); //background color
}