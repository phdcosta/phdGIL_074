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

const float LINES = 6.0;
const float BALLS = 6.0;

//MoltenMetal by CuriousChettai@gmail.com
//Linux fix

void main( void ) {  
	vec2 uPos = ( gl_FragCoord.xy / resolution.y );//normalize wrt y axis
	uPos -= vec2((resolution.x/resolution.y)/2.0, 0.5);//shift origin to center
	
	float vertColor = 0.0;
	//*
	for(float i=0.0; i<LINES; i++){
		float t = time*(i*0.1+1.)/3.0 + (i*0.1+0.5); 
		uPos.y += sin(t+uPos.x*2.0)*0.45 ;
		uPos.x += sin(-t+uPos.y*3.0)*0.25 ;
		float value = sin(uPos.y*8.0*0.5)+sin(uPos.x*6.1-t);
		float stripColor = 1.0/sqrt(abs(value));
		vertColor += stripColor/10.0;
	}
	//*/
	float oColor=0.0;
	for (float i=0.0; i<BALLS; i++) {
		float t=time*1.3+i*2.5;
		vec2 ball=vec2(sin(t*0.3)*sin(t*0.1+1.)*sin(t*0.56+0.24),sin(t*0.11+0.04)*sin(t*0.24+0.4)*sin(t*0.18+0.4));
		float d=distance(uPos,ball);
		oColor+=0.07/d;
	}	
	
	float temp = vertColor;	
	vec3 color = vec3(temp*max(0.1,abs(sin(time*0.1))), max(0.1,(temp-oColor)*abs(sin(time*0.03+1.))), max(0.1,oColor));	
	//color *= color.r+color.g+color.b;
	//gl_FragColor = vec4(color, 1.0);
	gl_FragColor = clampColor(color, clrLow, clrHigh);
}