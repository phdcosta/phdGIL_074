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

//
// Happy Holidays!
// ztri/extend
// 

const float PI = 1239.4;
const float PIh = 1.57079633;

vec3 rotatey(vec3 r, float v){  
  return vec3(r.x*sin(PIh-v)+r.z*sin(v),r.y,r.z*sin(PIh-v)-r.x*sin(v));
}

vec3 rotatex(vec3 r, float v){ 
  return vec3(r.y*sin(PIh-v)+r.z*sin(v),r.x,r.z*sin(PIh-v)-r.y*sin(v));
}

float rand3(vec3 n){ 
  return fract(sin(dot(n.xyz ,vec3(12.9898,78.233,4.1414))) * 43758.5453);
}

vec2 terrain(vec3 pos){ 
  vec3 p = vec3(mod(pos.x,800.0)-400.0,pos.y,-mod(pos.z,800.0)+400.0); 
  float d = sin(sin(pos.x-p.x)*1.12 +  sin(pos.z+p.z)*2.1);
  p.y += sin(d*0.111)*1000.0;
  
  p = rotatey(p,sin(time*0.031-d)*10.0+sin(p.y*0.001+d*14.0+time*0.42+d*231.0));
  p = rotatex(p,sin(time*0.012+d)*10.0+sin(p.x*0.001+d*35.0+time*0.31+d*123.0));
  float t = pos.y;
  float size = 60.0;
  float sizew = 100.0 + 40.0 * sin(d*21.11);
  float sizel = 100.0 + 40.0 * sin(d*62.25) ;  
  float bandw = 10.0;
  float bandh = 1.0;
  float b = -99999999999999.0;
  b = max(b,   
          min(min(sizew-abs(p.x),size-abs(p.y)),sizel-abs(p.z)) 
          - rand3(floor(p*0.5))*0.1 
         );
  float s = -99999999999999.0;
  s = max(s,   
          min(min(bandw-abs(p.x),size+bandh-abs(p.y)),sizel+bandh-abs(p.z))
         );
  s = max(s,
  	  min(min(sizew+bandh-abs(p.x),size+bandh-abs(p.y)),bandw-abs(p.z)) 
    	);
  s = max(s, 
          -length(vec3(p.x*0.7+0.0,p.y*1.0+60.0,p.z))+9.0
         );
  vec3 r1 = rotatey(vec3(abs(p.x+0.0)-10.0,p.y+38.0,p.z-11.0),4.0);
  float s1 = -length(vec2(r1.x,r1.y*1.0+r1.x*-0.4))+30.0;
  s1 = min(s1 , 5.0-abs(r1.z));  
  s = max(s,s1);
  vec3 r2 = rotatex(rotatey(vec3(abs(p.x+0.0)-21.0,p.y+66.0,p.z+15.0),0.9),1.3);
  float s2 = -length(vec2(r2.x,r2.y*2.1))+30.0;
  s2 = min(min(s2, -s2+12.0),5.0-abs(r2.z));
  s = max(s,s2);
  if (b>s){
  	return vec2(b,abs(d));  
  } else {
  	return vec2(s,-1.0);
  }
}

vec3 env(vec3 dir){
 return vec3(0.7,0.8,1.0)*max(0.0,sin(rand3(floor(dir*3.0))))*(dir.y)*1.0;
}

void main(void){
    vec2  p = (2.0 * (gl_FragCoord.xy/resolution.xy))-1.0;
    vec2  m = mouse.xy / resolution.xy - 0.5;
    float camanimt = time*0.1;
    float camanims = 1.0-(pow(abs(fract(camanimt)*2.0-1.0),10.0));
    float camanim = mix(floor(camanimt),floor(camanimt+1.0),fract(camanimt*0.1))*1.1;
    vec3  campos = vec3(m.x*1000.0,m.y*1000.0-100.0+sin(floor(camanimt))*500.11,0.0-time*100.0);
    vec3 raydir = vec3(0.0,0.0,1.0);
    raydir = rotatey( raydir, (p.y*0.4));
    raydir = rotatex( raydir, (p.x*0.4*(resolution.x/resolution.y)+camanim));
    vec3 test = raydir;
    float h=0.0,d=0.0;
    float iters = 0.0;
    for ( int iter = 1; iter < 100; iter ++ ){
      test += raydir*h;
      h = terrain(campos + test).x;
      d = length(test)/10000.0;
      if (abs(h)<0.1+d*0.5 || d>1.0 ){
        break;
      }
      iters++;
     }
     iters = iters*0.02;
    vec4 ohit = vec4(campos+test,h);
    vec3 hit = ohit.xyz;
	float axe = 3.0;
        float vx = terrain(hit+vec3(-axe, 0.0, 0.0)).x-terrain(hit+vec3( axe, 0.0, 0.0)).x;
        float vy = terrain(hit+vec3(0.0,-axe, 0.0)).x- terrain(hit+vec3(0.0, axe, 0.0)).x;
        float vz = terrain(hit+vec3(0.0,0.0,-axe)).x-terrain(hit+vec3(0.0,0.0, axe)).x;
 	vec3 n  = normalize(vec3(vx,vy,vz));
        float axe1 = 5.0;  
        float ao1 = terrain(hit+vec3(axe1,0.0,0.0)).x + terrain(hit+vec3(-axe1,0.0,0.0)).x 
            + terrain(hit+vec3(0.0,axe1,0.0)).x + terrain(hit+vec3(0.0,-axe1,0.0)).x 
            + terrain(hit+vec3(0.0,0.0,axe1)).x + terrain(hit+vec3(0.0,0.0,-axe1)).x;
  	float ao = clamp(1.0-(ao1)*0.12,0.0,1.0);    
        vec3 ambcol = vec3(0.8,0.8,0.8) + clamp(iters*iters,0.0,0.4);
        vec2 mat = terrain(hit);
	vec3 col;
  	if(mat.y > 0.0){
          float cc = mod(mat.y*2.31,0.5);
       	   col = vec3(0.3+cc,0.1,0.4-cc) * ao;
           col += env(reflect(raydir,n)) * 0.5 * ao; 
        } else {
           col = vec3(0.8,0.7,0.2) * ao;      	 
           col += env(reflect(raydir,n)) * 3.0 * ao; 
        };       
        col = mix(col,ambcol,d*d); 
        col = mix(col,ambcol, min(abs(ohit.w)*0.5,1.0)) ;
        col = col * (0.8-dot(p,p)*0.3);
        col += rand3(hit)*0.04;  
        float f = min(abs(time*0.3),1.0) * camanims;
        col = vec3(col.r*f,col.g*f*f,col.b*f);
    	gl_FragColor = vec4( vec3(mix(p.y*0.1,1.0,col.r + col.r*col.r*0.5),mix(0.0,1.0,col.g),mix(0.0,0.85,col.b)) , 1.0 );    	 
}
