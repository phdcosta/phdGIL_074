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

const float COUNT = 10.0;

//MoltenMetal by CuriousChettai@gmail.com
//Linux fix

void main( void ) {  
	vec2 uPos = ( gl_FragCoord.xy / resolution.y );//normalize wrt y axis
	uPos -= vec2((resolution.x/resolution.y)/2.0, 0.5);//shift origin to center
	
	float vertColor = 0.0;
	for(float i=0.0; i<COUNT; i++){
		float t = time/3.0 + (i*3.); 
		uPos.y += sin(-t+uPos.x*2.0)-sin(t)*0.1;
		uPos.x += cos(-t+uPos.y*3.0+cos(t))*0.15;
		float value = (sin(uPos.y*10.0)+sin(i*0.1) + uPos.x*5.1);
		
		//float d=1./pow(distance(mouse,uPos),2.);
		
		float stripColor = 1.0/sqrt(abs(value))*(abs(sin(time*0.1+2.))*0.5+2.0);
		
		vertColor += stripColor/30.0;
	}
	
	float temp = vertColor;	
	vec3 color = vec3(temp*max(0.3,sin(time*0.1)), max(0.1,temp*sin(time*0.05+1.)), max(0.1,temp*sin(time*0.02)));	
	color *= color.r+color.g+color.b;
//	gl_FragColor = vec4(color, 1.0);
	gl_FragColor = clampColor(color, clrLow, clrHigh);
}