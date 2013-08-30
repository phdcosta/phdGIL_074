#version 120

#ifdef GL_ES
precision mediump float;
#endif

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

//Energize!
//Timing is still off...

//Added Rotation
//Added some color
//Added mouse input

vec2 rotate(vec2 point, float rads) {
	float cs = cos(rads);
	float sn = sin(rads);
	return point * mat2(cs, -sn, sn, cs);
}	


void main( void ) {
	
	float rads = radians(time*3.15);
	vec2 position = ( gl_FragCoord.xy -  resolution.xy*.5 ) / resolution.x;
	
	// 256 angle steps
	float angle = atan(position.y,position.x)/(2.*3.14159265359)+rads;
	angle -= floor(angle);
	float rad = length(position);
	
	float color = 0.0;
	for (int i = 0; i < 10; i++) {
		angle += mouse.x / resolution.x * 0.1;
		float angleFract = fract(angle*256.);
		float angleRnd = floor(angle*256.)+1.;
		float angleRnd1 = fract(angleRnd*fract(angleRnd*.7235)*45.1);
		float angleRnd2 = fract(angleRnd*fract(angleRnd*.82657)*13.724);
		float t = time+angleRnd1*10.0+ sin(time*0.5);
		float radDist = sqrt(angleRnd2+float(i));
		
		float adist = radDist/rad*.1;
		float dist = (t*.1+adist);
		dist = abs(fract(dist)-.5);
		color += max(0.01,1.5-dist*(30.+20.*sin(time))/adist)*(.5-abs(angleFract-.5))*5./adist/radDist;
		
		angle = fract(angle+.61);
	}

	float color1 = color*rad;
	gl_FragColor = vec4( color1,color1,color,color1)*.3;
}