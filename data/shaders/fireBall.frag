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
const int _MaxSteps = 64;
const float _StepDistanceScale = 0.5;
const float _MinStep = 0.001;
const float _DistThreshold = 0.005;
const int _VolumeSteps = 32;
const float _StepSize = 0.02;
const float _Density = 0.1;
const float _SphereRadius = 0.5;
const float _NoiseFreq = 4.0;
const float _NoiseAmp = -0.5;
const vec3 _NoiseAnim = vec3(1, 0, 0);
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
	p = m*p*2.02; f += 0.03125*abs(noise( p ));
	return f/0.9375;
}

float sphereDist(vec3 p, vec4 sphere) {
	return length(p - sphere.xyz) - sphere.w;
}

float distanceFunc(vec3 p, out float displace) {
	float d = length(p) - _SphereRadius;
	d = length(p) - mouse.x / resolution.x + 0.1;
	displace = fbm(p*_NoiseFreq + _NoiseAnim*time);
	d += displace * _NoiseAmp;
	return d;
}

vec3 dfNormal(vec3 pos) {
	float eps = 0.001;
	vec3 n;
	float s;
	const int i = 1;
	if(i == 0) {
		n.x = distanceFunc( vec3(pos.x+eps, pos.y, pos.z), s ) - distanceFunc( vec3(pos.x-eps, pos.y, pos.z), s );
		n.y = distanceFunc( vec3(pos.x, pos.y+eps, pos.z), s ) - distanceFunc( vec3(pos.x, pos.y-eps, pos.z), s );
		n.z = distanceFunc( vec3(pos.x, pos.y, pos.z+eps), s ) - distanceFunc( vec3(pos.x, pos.y, pos.z-eps), s );
	} else {
		float d = distanceFunc(pos, s);
		n.x = distanceFunc( vec3(pos.x+eps, pos.y, pos.z), s ) - d;
		n.y = distanceFunc( vec3(pos.x, pos.y+eps, pos.z), s ) - d;
		n.z = distanceFunc( vec3(pos.x, pos.y, pos.z+eps), s ) - d;
	}
	return normalize(n);
}

vec4 gradient(float x) {
	const vec4 c0 = vec4(4, 4, 4, 1);
	const vec4 c1 = vec4(1, 1, 0, 1);
	const vec4 c2 = vec4(1, 0, 0, 1);
	const vec4 c3 = vec4(0.4, 0.4, 0.4, 4);
	float t = fract(x*3.0);
	vec4 c;
	if (x < 0.3333) {
		c =  mix(c0, c1, t);
	} else if (x < 0.6666) {
		c = mix(c1, c2, t);
	} else {
		c = mix(c2, c3, t);
	}
	return c;
}

vec4 shade(vec3 p, float displace) {
	displace = displace*1.5 - 0.2;
	displace = clamp(displace, 0.0, 0.99);
	vec4 c = gradient(displace);
	vec3 n = dfNormal(p);
	float diffuse = n.z*0.5+0.5;
	c.rgb = mix(c.rgb, c.rgb*diffuse, clamp((displace-0.5)*2.0, 0.0, 1.0));
	return c;
}

vec4 volumeFunc(vec3 p) {
	float displace;
	float d = distanceFunc(p, displace);
	vec4 c = shade(p, displace);
	return c;
}

vec3 sphereTrace(vec3 rayOrigin, vec3 rayDir, out bool hit, out float displace) {
	vec3 pos = rayOrigin;
	hit = false;
	displace = 0.0;
	float d;
	float disp;
	for(int i=0; i<_MaxSteps; i++) {
		d = distanceFunc(pos, disp);
      		if (d < _DistThreshold) {
			hit = true;
			displace = disp;
      		}
		pos += rayDir*d*_StepDistanceScale;
	}
	return pos;
}

vec4 rayMarch(vec3 rayOrigin, vec3 rayStep, out vec3 pos) {
	vec4 sum = vec4(0, 0, 0, 0);
	pos = rayOrigin;
	for(int i=0; i<_VolumeSteps; i++) {
		vec4 col = volumeFunc(pos);
		col.a *= _Density;
		col.a = min(col.a, 1.0);
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
	float rotx = mouse.y/resolution.y*4.0;
	float roty = mouse.x/resolution.x*4.0;
	vec3 ro = 2.5*normalize(vec3(cos(roty), cos(rotx), sin(roty)));
	vec3 ww = normalize(vec3(0.0,0.0,0.0) - ro);
	vec3 uu = normalize(cross( vec3(0.0,1.0,0.0), ww ));
	vec3 vv = normalize(cross(ww,uu));
	vec3 rd = normalize( p.x*uu + p.y*vv + 1.5*ww );
	bool hit;
	float displace;
	vec3 hitPos = sphereTrace(ro, rd, hit, displace);
	vec4 col = vec4(0, 0, 0, 1);
	if (hit) {
		col = shade(hitPos, displace);
	}
	gl_FragColor = clampColor(col.rgb, clrLow, clrHigh);
}
