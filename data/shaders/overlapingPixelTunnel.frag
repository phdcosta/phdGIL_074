// by rotwang, overlapping pixels in different sizes.
// drifting around by @emackey
// "tunnelized" by kabuto
// rotwang: @mod* variation

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
const float PI = 3.1415926535;
const float TWOPI = PI*2.0;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(11.9898,78.233))) * 43758.5453);
}

void main( void ) {

	vec2 dir = vec2(0, sin(time * 0.1) * .15);
	vec2 travel = time * vec2(-0.04, 0);
	float aspect = resolution.x / resolution.y;
	vec2 p = (gl_FragCoord.xy-mouse.xy)/resolution.xy;

	//p = p + (-mouse/resolution) + .75;

	float angle = (atan(p.x, p.y)+PI)/TWOPI;
	vec2 pos1 = vec2(-.1/length(p),atan(p.y,p.x)/(2.*3.1415926));
	vec2 pos = pos1 + dir + travel;
	
	vec3 clr = vec3(0.0);
	for(int i=0;i<8;i++)
	{
		pos.y = fract(pos.y);
		float n = pow(4.0-float(i), 2.0)*2.;
		vec2 pos_z = floor(pos*n);
		float a = 1.0-step(0.1, rand(pos_z))*angle*2.0;
		float rr = rand(pos_z)*a;
		float gg = rand(pos_z+n)*a;
		float bb = rand(pos_z+n+n)*a;
		
		vec3 clr_a = vec3(rr, gg, bb);
		clr += clr_a*clr_a/4.0;
		pos += dir + travel;
	}
	
	//clr = sqrt(clr);
	clr *= -pos1.x;
	clr += length(pos1)*vec3(0.9, 0.3,0.2);
	clr *= 1.0-length(p*p);
	gl_FragColor = clampColor( clr, clrLow, clrHigh );
	
}