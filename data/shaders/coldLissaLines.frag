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

const float COUNT = 40.0;

//SpiderWeb by CuriousChettai@gmail.com
//Linux fix
// [06 Sept 12] Spyke - Added some simple mouse control

void main( void ) {  
	vec2 uPos = ( gl_FragCoord.xy / resolution.y );//normalize wrt y axis
	vec2 mPos;
	mPos.x = 0.3 + 0.2 * (fftA.x + fftB.x + fftA.y + fftB.y) / 4.0;
	mPos.y = 0.2 + 0.8 * (fftA.z + fftB.z + fftA.w + fftB.w) / 4.0;
//	mPos = (mouse / resolution.y);
	uPos -= vec2((resolution.x/resolution.y)/2.0, 0.5);//shift origin to center
	
	vec3 color = vec3(0);
	
	float vertColor = 0.0;
	for(float i=0.0; i<COUNT; i++){
		float t = time/10.0 + (i+0.3); 
		
		uPos.y += sin(-t+uPos.x*(mPos.x*10.0))*0.2;
		float xAmp = sin(-t+uPos.y*(mPos.y*10.0)+cos(t/1.0))*0.2;
		float value = (sin(uPos.y*10.0) + uPos.x*xAmp*100.0);
		
		float stripColor = 1.0/sqrt(abs(value))/20.0;
		
		vertColor += stripColor;
		
		color = vec3(vertColor*0.01, vertColor*0.1, vertColor);
	}
	
	color *= color.r+color.g+color.b;
	gl_FragColor = clampColor(color, clrLow, clrHigh);
}