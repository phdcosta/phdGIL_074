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

// mods by dist, shrunk slightly by @danbri
// Extended to show Fannys by @TimIsMint,@BruceIsAlsoMint

void main(void) {
	vec2 uPos = ( gl_FragCoord.xy / (resolution.xy));//normalize with rt y axis
	uPos.y += .5; //
	uPos -= .5;
	vec2 mPos = mouse.xy/resolution.xy;
	vec3 color = vec3(0.0);
	float vertColor = 0.0;
	for( float i = 0.; i < 8.; ++i ) {
		// uPos.y += sin( uPos.x * i + (.39 * TIME * I^2 )
		uPos.y += sin( uPos.x*(i) + (time*1.3 * i * i * 0.3) ) * 0.15*mPos.y;
		float fTemp = abs(0.5 / uPos.y / 90.0) * (mPos.x+0.1)/10.0*6.0;
		vertColor += fTemp;
		color += vec3( fTemp*(7.0-i)/10.0, (fTemp*i/3.0)*(max(sin(time*0.5)*5.0,0.3)), pow(fTemp,0.9)*1.5 );
	}
	gl_FragColor = vec4(color, 1.0);
}
