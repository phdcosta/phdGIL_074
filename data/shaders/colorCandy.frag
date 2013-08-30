// Vlt / Dfm 
// Grid function from here: http://www.cs.uaf.edu/2010/spring/cs481/section/2/lecture/04_13_procedural.html
// replaced texture2D with noise function, then played around a bit.

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

vec2 noise(vec2 n) {
	vec2 ret;
	ret.x=fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453);
	ret.y=fract(cos(dot(n.yx, vec2(34.9865, 65.946)))* 28618.3756);
	return ret;
}

float grid(vec2 loc) {
	float dist=10.; // distance to closest grid cell
	vec2 gridcorner=floor(loc);
	for (float dy=-1.0;dy<=1.0;dy++)
		for (float dx=-1.0;dx<=1.0;dx++)
		{
			vec2 thiscorner=gridcorner+vec2(dx,dy);
			vec2 gridshift=noise(thiscorner);
			if (gridshift.x>.5) gridshift.x = .8;else gridshift.x=0.2; // pick some weird grid offsets
			if (gridshift.y>.5) gridshift.y = .1;else gridshift.y=0.7; // pick some weird grid offsets
			//vec2 center=thiscorner+gridshift;
			//vec2 delta = loc - center;
			//float d = abs(delta.x)+abs(delta.y);
			//dist=min(d,dist);
			vec2 center=thiscorner+gridshift;
			float radius=length(loc-center);
			dist=min(radius,dist);
		}
	return dist;
}

vec3 hsv(float h,float s,float v) {
	return mix(vec3(1.),clamp((abs(fract(h+vec3(3.,2.,1.)/3.)*6.-3.)-1.),0.,1.),s)*v;
}

void main( void ) {

	vec2 position = (( gl_FragCoord.xy / resolution.y )*2.0)-1.0;
	
	// get cellular
	float c=grid(position*2.0);
	
	// emphasize bright and dark
	//c=pow(c*2.0,2.)*0.5;

	//gl_FragColor = c > 0.98 ? vec4(c*.1, 0.0, 0.0, 1.0)  : vec4(c*.1);
	gl_FragColor = vec4(hsv(sin(c*15.-time)*.5+.5,1.,1.),1.);
}