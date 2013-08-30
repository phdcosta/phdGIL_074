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

float F2 = 0.366025403;
float G2 = 0.211324865;
float K = .0243902439;
float permute(float x) {
	return mod((34.0 * x + 1.)*x, 289.0);
}

vec2 grad2(vec2 p, float rot) {
	int i = 1;
	if(i == 1) {
		float u = permute(permute(p.x) + p.y) * K + rot;
		u = 4.0 * fract(u) - 2.0;
		return vec2(abs(u)-1.0, abs(abs(u+1.0)-2.0)-1.0);
	} else {
		float u = permute(permute(p.x) + p.y) * K + rot;
		u = fract(u) * 6.28318530718;
		return vec2(cos(u), sin(u));
	}
}

float srdnoise(in vec2 P, in float rot, out vec2 grad) {
	vec2 Ps = P + dot(P, vec2(F2));
	vec2 Pi = floor(Ps);
	vec2 P0 = Pi - dot(Pi, vec2(G2));
	vec2 v0 = P - P0;
	vec2 i1 = (v0.x > v0.y) ? vec2(1.0, 0.0) : vec2 (0.0, 1.0);
	vec2 v1 = v0 - i1 + G2;
	vec2 v2 = v0 - 1.0 + 2.0 * G2;
	Pi = mod(Pi, 289.0);
	vec3 t = max(0.5 - vec3(dot(v0,v0), dot(v1,v1), dot(v2,v2)), 0.0);
	vec3 t2 = t*t;
	vec3 t4 = t2*t2;
	vec2 g0 = grad2(Pi, rot);
	vec2 g1 = grad2(Pi + i1, rot);
	vec2 g2 = grad2(Pi + 1.0, rot);
	vec3 gv = vec3(dot(g0,v0), dot(g1,v1), dot(g2,v2));
	vec3 n = t4 * gv;
	vec3 temp = t2 * t * gv;
	vec3 gradx = temp * vec3(v0.x, v1.x, v2.x);
	vec3 grady = temp * vec3(v0.y, v1.y, v2.y);
	grad.x = -8. * (gradx.x + gradx.y + gradx.z);
	grad.y = -8. * (grady.x + grady.y + grady.z);
	grad.x += dot(t4, vec3(g0.x, g1.x, g2.x));
	grad.y += dot(t4, vec3(g0.y, g1.y, g2.y));
	grad *= 40.;
	return 40. * (n.x + n.y + n.z);
}

void main(void) {
	vec2 g1, g2;
	vec2 p = ( gl_FragCoord.xy - (mouse))/80.*(sin(time*.15)+2.);
	float n1 = srdnoise(p*0.5, 0.2*time, g1);
	float n2 = srdnoise(p*2.0 + g1*0.5, 0.51*time, g2);
	float n3 = srdnoise(p*4.0 + g1*0.5 + g2*.25, 0.77*time, g2);
	gl_FragColor = clampColor(vec3(.9, 0.25, 0.01) + vec3(n1+.95*n2+.75*n3), clrLow, clrHigh);
}
