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

const float fScale=0.05;
const float fHeight=100.0;
const float fEps=10.0;
const vec3 cBackground1=vec3(0.1,0.5,0.8);
const vec3 cBackground2=vec3(0.1,0.5,0.8);
const vec3 cAmbient=vec3(0.1,0.1,0.1);
const vec3 cDiffuse=vec3(0.1,0.5,0.3);
const vec3 cSpecular=vec3(1.0,1.0,1.0);
const vec3 cLight=vec3(1.0,1.0,0.0);
const vec3 cFlare=vec3(0.9,0.3,3.0);
const vec3 cVignette=vec3(0.0,0.0,0.0);
const float fBackground=0.004;
const float fAmbient=0.0;
const float fDiffuse=1.0;
const float fSpecularity=0.3;
const float fHardness=8.0;
const float fIntensity=1000.0;
const float fOpacity=0.8;
const float fRefract=0.05;
const float fFlareSize=500.0;
const float fFlareOpacity=0.4;
const float fVignette=1.0;
const float fRadius=0.7;
vec3 mod289(vec3 x) {
	return x-floor(x*(1.0/289.0))*289.0;
}

vec4 mod289(vec4 x) {
	return x-floor(x*(1.0/289.0))*289.0;
}

vec4 permute(vec4 x) {
	return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r) {
	return 1.79284291400159-0.85373472095314*r;
}

vec3 fade(vec3 t) {
	return t*t*t*(t*(t*6.0-15.0)+10.0);
}

float pnoise(vec3 P) {
	vec3 Pi0=floor(P);
	vec3 Pi1=Pi0+vec3(1.0);
	Pi0=mod289(Pi0);
	Pi1=mod289(Pi1);
	vec3 Pf0=fract(P);
	vec3 Pf1=Pf0-vec3(1.0);
	vec4 ix=vec4(Pi0.x,Pi1.x,Pi0.x,Pi1.x);
	vec4 iy=vec4(Pi0.yy,Pi1.yy);
	vec4 iz0=Pi0.zzzz;
	vec4 iz1=Pi1.zzzz;
	vec4 ixy=permute(permute(ix)+iy);
	vec4 ixy0=permute(ixy+iz0);
	vec4 ixy1=permute(ixy+iz1);
	vec4 gx0=ixy0*(1.0/7.0);
	vec4 gy0=fract(floor(gx0)*(1.0/7.0))-0.5;
	gx0=fract(gx0);
	vec4 gz0=vec4(0.5)-abs(gx0)-abs(gy0);
	vec4 sz0=step(gz0,vec4(0.0));
	gx0-=sz0*(step(0.0,gx0)-0.5);
	gy0-=sz0*(step(0.0,gy0)-0.5);
	vec4 gx1=ixy1*(1.0/7.0);
	vec4 gy1=fract(floor(gx1)*(1.0/7.0))-0.5;
	gx1=fract(gx1);
	vec4 gz1=vec4(0.5)-abs(gx1)-abs(gy1);
	vec4 sz1=step(gz1,vec4(0.0));
	gx1-=sz1*(step(0.0,gx1)-0.5);
	gy1-=sz1*(step(0.0,gy1)-0.5);
	vec3 g000=vec3(gx0.x,gy0.x,gz0.x);
	vec3 g100=vec3(gx0.y,gy0.y,gz0.y);
	vec3 g010=vec3(gx0.z,gy0.z,gz0.z);
	vec3 g110=vec3(gx0.w,gy0.w,gz0.w);
	vec3 g001=vec3(gx1.x,gy1.x,gz1.x);
	vec3 g101=vec3(gx1.y,gy1.y,gz1.y);
	vec3 g011=vec3(gx1.z,gy1.z,gz1.z);
	vec3 g111=vec3(gx1.w,gy1.w,gz1.w);
	vec4 norm0=taylorInvSqrt(vec4(dot(g000,g000),dot(g010,g010),dot(g100,g100),dot(g110,g110)));
	g000*=norm0.x;
	g010*=norm0.y;
	g100*=norm0.z;
	g110*=norm0.w;
	vec4 norm1=taylorInvSqrt(vec4(dot(g001,g001),dot(g011,g011),dot(g101,g101),dot(g111,g111)));
	g001*=norm1.x;
	g011*=norm1.y;
	g101*=norm1.z;
	g111*=norm1.w;
	float n000=dot(g000,Pf0);
	float n100=dot(g100,vec3(Pf1.x,Pf0.yz));
	float n010=dot(g010,vec3(Pf0.x,Pf1.y,Pf0.z));
	float n110=dot(g110,vec3(Pf1.xy,Pf0.z));
	float n001=dot(g001,vec3(Pf0.xy,Pf1.z));
	float n101=dot(g101,vec3(Pf1.x,Pf0.y,Pf1.z));
	float n011=dot(g011,vec3(Pf0.x,Pf1.yz));
	float n111=dot(g111,Pf1);
	vec3 fade_xyz=fade(Pf0);
	vec4 n_z=mix(vec4(n000,n100,n010,n110),vec4(n001,n101,n011,n111),fade_xyz.z);
	vec2 n_yz=mix(n_z.xy,n_z.zw,fade_xyz.y);
	return mix(n_yz.x,n_yz.y,fade_xyz.x);
}

float getDepth(const vec2 vPoint) {
	return ((pnoise(vec3(vPoint*fScale,time))+1.0)*0.5)*fHeight;
}

vec3 getNormal(const vec2 vPoint,const float fEps) {
	vec2 v=vec2(fEps,0.0);
	return normalize(vec3(
		getDepth(vPoint+v.xy)-getDepth(vPoint-v.xy),
		getDepth(vPoint+v.yx)-getDepth(vPoint-v.yx),
		getDepth(vPoint)));
}

float getBlinnPhong(const vec3 vPoint,const vec3 vLight,const vec3 vNormal,const float fSpecular,const float fHardness) {
	vec3 vHalf=normalize(vLight-vPoint);
	float fNdh=dot(vHalf,vNormal);
	return pow(max(0.0,fNdh),fHardness)*fSpecular;
}

float getLightIntensity(const vec2 vPoint,const vec3 vLight,const float fIntensity) {
	float fDist=length(vPoint-vLight.xy);
	return clamp((1.0/(fDist*fDist))*fIntensity,0.0,1.0);
}

vec3 getBackground(const vec2 vPoint) {
	return mix(cBackground2,cBackground1,clamp(vPoint.y*fBackground,0.0,1.0));
}

vec3 getColour(const vec2 vPoint) {
	vec2 vScreen=resolution;
	vec2 vMouse=mouse.xy / resolution.xy;
	vec3 vLight=vec3(vMouse*vScreen,fHeight);
	vec3 vNormal=getNormal(vPoint,fEps);
	float fSpecular=getBlinnPhong(vec3(vPoint,-fHeight),vLight,vNormal,fSpecularity,fHardness);
	float fLight=getLightIntensity(vPoint,vLight,fIntensity)*fOpacity;
	vec3 cDiffuse=getBackground(vPoint);
	return (cAmbient*fAmbient)+(cDiffuse*fDiffuse)+(cSpecular*fSpecular)+(cLight*fLight);
}

vec3 getRefractColour(const vec2 vPoint,const float fRefract) {
	vec2 vRefract=vPoint+((fRefract*2.0)-1.0);
	return getColour(vRefract);
}

vec3 getRefract(const vec2 vPoint,const float fRefract) {
	return vec3(
		getRefractColour(vPoint,0.0).r,
		getRefractColour(vPoint,fRefract).g,
		getRefractColour(vPoint,fRefract*2.0).b);
}

float getFlare(const vec2 vPoint,const vec3 vLight,const float fPosition,const float fSize,const float fOpacity) {
	vec2 vScreen=resolution;
	if(vLight.x<0.0 || vLight.x>vScreen.x || vLight.y<0.0 || vLight.y>vScreen.y) return 0.0;
	vec2 vFlare=(vScreen-vLight.xy);
	vec2 vDir=normalize(vLight.xy-vFlare);
	float fLen=length(vLight.xy-vFlare);
	vFlare+=vDir*fLen*fPosition;
	float fDist=length(vPoint-vFlare);
	float fStrength=(1.0-(fLen/(min(vScreen.x,vScreen.y)*2.0)))*fOpacity;
	return clamp((1.0/(fDist*fDist))*fSize,0.0,1.0)*fStrength;
}

vec3 getVignette(const vec2 vPoint,const float fVignette,const float fRadius,const vec3 cVignette,const vec3 cColour) {
	vec2 vScreen=resolution;
	vec2 vVignette=(vPoint/vScreen)-0.5;
	return mix(cColour,mix(cColour,cVignette,pow(length(vVignette)/fRadius,4.0)),fVignette);
}

void main(void) {
	vec2 vScreen=resolution;
	vec2 vMouse=mouse.xy / resolution.xy;
	vec2 vPoint=gl_FragCoord.xy;
	vec3 vLight=vec3(vMouse*vScreen,fHeight);
	vec3 cColour=getRefract(vPoint,getDepth(vPoint)*fRefract);
	float fFlare=(
		getFlare(vPoint,vLight,0.0,fFlareSize*0.1,fFlareOpacity*1.5)+
		getFlare(vPoint,vLight,0.1,fFlareSize*0.5,fFlareOpacity)+
		getFlare(vPoint,vLight,0.3,fFlareSize,fFlareOpacity*1.5)+
		getFlare(vPoint,vLight,0.65,fFlareSize*0.1,fFlareOpacity*1.6)+
		getFlare(vPoint,vLight,0.7,fFlareSize*0.2,fFlareOpacity)+
		getFlare(vPoint,vLight,0.8,fFlareSize*2.5,fFlareOpacity*1.4)+
		getFlare(vPoint,vLight,0.9,fFlareSize*0.3,fFlareOpacity*1.5));
	cColour+=cFlare*fFlare;
	gl_FragColor = clampColor(getVignette(vPoint,fVignette,fRadius,cVignette,cColour), clrLow, clrHigh);
}
