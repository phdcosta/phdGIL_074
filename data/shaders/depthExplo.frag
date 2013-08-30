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

const float _OpacityThreshold = 1.0;
const int _VolumeSteps = 32;
const float _StepSize = 0.05;
const float _Density = 0.2;
const float _SphereRadius = 2.0;
const float _NoiseFreq = 2.0;
const float _NoiseAmp = 2.0;
const vec3 _NoiseAnim = vec3(0, 0, 0);
mat3 m = mat3( 0.00,  0.80,  0.60, -0.80,  0.36, -0.48, -0.60, -0.48,  0.64 );

float hash( float n ) {
	return fract(sin(n)*43758.5453);
}

float noise( in vec3 x ) {
	vec3 p = floor(x);
	vec3 f = fract(x);
	f = f*f*(3.0-2.0*f);
	float n = p.x + p.y*57.0 + 113.0*p.z;
	float res = mix(mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),
					mix( hash(n+ 57.0), hash(n+ 58.0),f.x),f.y),
					mix(mix( hash(n+113.0), hash(n+114.0),f.x),
					mix( hash(n+170.0), hash(n+171.0),f.x),f.y),f.z);
	return res;
}

float fbm( vec3 p ) {
	float f;
	f = 0.5000*noise( p ); p = m*p*2.02;
	f += 0.2500*noise( p ); p = m*p*2.03;
	f += 0.1250*noise( p ); p = m*p*2.01;
	f += 0.0625*noise( p );
	return f;
}

float distanceFunc(vec3 p) {
	float d = length(p) - _SphereRadius;
	d += fbm(p*_NoiseFreq + _NoiseAnim*time) * _NoiseAmp;
	return d;
}

vec4 gradient(float x) {
	x=sin(x-time);
	const vec4 c0 = vec4(2, 2, 1, 0.1);
	const vec4 c1 = vec4(1, 0, 0, 0.9);
	const vec4 c2 = vec4(0, 0, 0, 0);
	const vec4 c3 = vec4(0, 0.5, 1, 0.2);
	const vec4 c4 = vec4(0, 0, 0, 0);
	x = clamp(x, 0.0, 0.999);
	float t = fract(x*4.0);
	vec4 c;
	if (x < 0.25) {
		c =  mix(c0, c1, t);
	} else if (x < 0.5) {
		c = mix(c1, c2, t);
	} else if (x < 0.75) {
		c = mix(c2, c3, t);
	} else {
		c = mix(c3, c4, t);
	}
	return c;
}

vec4 shade(float d) {
	vec4 c = gradient(d);
	return c;
}

vec4 volumeFunc(vec3 p) {
	float d = distanceFunc(p);
	return shade(d);
}

vec4 rayMarch(vec3 rayOrigin, vec3 rayStep, out vec3 pos) {
	vec4 sum = vec4(0, 0, 0, 0);
	pos = rayOrigin;
	for(int i=0; i<_VolumeSteps; i++) {
		vec4 col = volumeFunc(pos);
		col.a *= _Density;
		col.rgb *= col.a;
		sum = sum + col*(1.0 - sum.a);
     			if (sum.a > _OpacityThreshold) break;
		pos += rayStep;
	}
	return sum;
}

void main(void) {
	vec2 q = gl_FragCoord.xy / resolution.xy;
	vec2 p = -1.0 + 2.0 * q;
	p.x *= resolution.x/resolution.y;
	float rotx = mouse.y/resolution.y*4.0+sin(time);
	float roty = - mouse.x/resolution.x*4.0+cos(time);
	float zoom = 4.0;
	vec3 ro = zoom*normalize(vec3(cos(roty), cos(rotx), sin(roty)));
	vec3 ww = normalize(vec3(0.0,0.0,0.0) - ro);
	vec3 uu = normalize(cross( vec3(0.0,1.0,0.0), ww ));
	vec3 vv = normalize(cross(ww,uu));
	vec3 rd = normalize( p.x*uu + p.y*vv + 1.5*ww );
	ro += rd*2.0;
	vec3 hitPos;
	vec4 col = rayMarch(ro, rd*_StepSize, hitPos);
	gl_FragColor = clampColor(col.rgb, clrLow, clrHigh);
}

