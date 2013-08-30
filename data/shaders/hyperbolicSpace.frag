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

// A hyperbolic space renderer by Kabuto
// Modified, added some nice reflections :)

// Hold your mouse pointer near the left edge to look forward, near the center to look sideways and near the right edge to look backward

// Glass mod: structures filled with colored glass. Only the glass volume itself is colored, not its surface.

const float refractionIndex = 1.4;	// 1 = just fog, 1.2 = lightweight glass (most interesting value IMO), 1.4 = glass, 2.4 = diamond, 100 = apparently static texture
const float maxrad2 = 0.53;		// radius of innermost spheres
const float maxrad3 = 0.57;		// radius of outermost spheres

const float expFalloffInner = 2.5;		// exponential falloff inside material
const float expFalloffOuter = .5;		// exponential falloff outside material

// derived constants	

const float refractMed = (refractionIndex + 1./refractionIndex)*.5;
const float refractDiff = (refractionIndex - 1./refractionIndex)*.5; 
const float expFalloffMed = (expFalloffOuter + expFalloffInner)*.5;
const float expFalloffDiff = (expFalloffOuter - expFalloffInner)*.5;

const float a = 1.61803398874989484820; // (sqrt(5)+1)/2
const float b = 2.05817102727149225032; // sqrt(2+sqrt(5))
const float c = 1.27201964951406896425; // sqrt((sqrt(5)+1)/2)
const float d = 2.61803398874989484820; // (sqrt(5)+3)/2
const float e = 1.90211303259030714423; // sqrt((sqrt(5)+5)/2);

// Distance to the face of the enclosing polyhedron, given that all vectors are using klein metric
float kleinDist(vec3 pos, vec3 dir) {
	float q0 = dot(dir, vec3(a,+1.,0.));
	float l0 = (-dot(pos,vec3(a,+1.,0.)) + c*sign(q0)) / q0;
	float q1 = dot(dir, vec3(a,-1.,0.));
	float l1 = (-dot(pos,vec3(a,-1.,0.)) + c*sign(q1)) / q1;
	float q2 = dot(dir, vec3(0.,a,+1.));
	float l2 = (-dot(pos,vec3(0.,a,+1.)) + c*sign(q2)) / q2;
	float q3 = dot(dir, vec3(0.,a,-1.));
	float l3 = (-dot(pos,vec3(0.,a,-1.)) + c*sign(q3)) / q3;
	float q4 = dot(dir, vec3(+1.,0.,a));
	float l4 = (-dot(pos,vec3(+1.,0.,a)) + c*sign(q4)) / q4;
	float q5 = dot(dir, vec3(-1.,0.,a));
	float l5 = (-dot(pos,vec3(-1.,0.,a)) + c*sign(q5)) / q5;
	return min(min(min(l0,l1),min(l2,l3)),min(l4,l5));
}

// Distance to the nearest edge (klein metric) - albeit not used in this effect
float edgeDist(vec3 pos) {
	pos = abs(pos);
	vec3 o = c/a-max(pos, (pos.xyz*a + pos.yzx*(1.+a) + pos.zxy)/(2.*a));
	return min(min(o.x, o.y), o.z);
}

// Mirrors dir in the klein metric on the outer face of the polyhedron (on which pos must lie)
vec3 hreflect(vec3 pos, vec3 dir, inout vec4 fa, inout vec4 fb, inout vec4 fc) {
	vec3 s = sign(pos);
	vec3 apos2 = abs(pos);
	vec3 sdir = dir*s;
	vec3 q = apos2*a+apos2.yzx;
	if (q.x > q.y && q.x > q.z) {
		fa *= max(s.x*vec4(1,1,-1,-1),s.y*vec4(1,-1,1,-1));
		return normalize(pos*(c*sdir.y+b*sdir.x) + vec3(-a*(sdir.x+sdir.y),-a*sdir.x,sdir.z)*s);
	} else if (q.y > q.z) {
		fb *= max(s.y*vec4(1,1,-1,-1),s.z*vec4(1,-1,1,-1));
		return normalize(pos*(c*sdir.z+b*sdir.y) + vec3(sdir.x,-a*(sdir.y+sdir.z),-a*sdir.y)*s);
	} else {
		fc *= max(s.z*vec4(1,1,-1,-1),s.x*vec4(1,-1,1,-1));
		return normalize(pos*(c*sdir.x+b*sdir.z) + vec3(-a*sdir.z,sdir.y,-a*(sdir.z+sdir.x))*s);
	}
}

float sinh(float f) {
	return (exp(f)-exp(-f))*0.5;
}

vec4 kleinToHyper(vec3 klein) {
	return vec4(klein, 1.)*inversesqrt(1.-dot(klein,klein));
}

float hyperdist(vec4 a, vec4 b) {
	float lcosh = dot(a,b*vec4(-1,-1,-1,1));
	return log(lcosh+sqrt(lcosh*lcosh-1.));
}


void main( void ) {
	// Compute camera path and angle
	float f0 = fract(time*0.05)+1e-5;
	float f = fract(f0*2.);
	float fs = sign(f-.5);
	float fs0 = sign(f0-.5);
	vec3 dir = normalize(vec3(vec2(gl_FragCoord.x / resolution.x - 0.5, (gl_FragCoord.y - resolution.y * 0.5) / resolution.x), 0.5));

	vec2 mPos = mouse.xy / resolution.xy;
	
	float tc = cos((mPos.y-.5)*2.1);
	float ts = sin(-(mPos.y-.5)*2.1);
	float uc = cos((mPos.x-.1)*4.1);
	float us = sin(-(mPos.x-.1)*4.1);

	dir *= mat3(uc,-ts*us,-tc*us,0,tc,-ts,us,ts*uc,tc*uc);
	//dir *= vec3(sign(f-.5),sign(f-.5),1.);
	dir.z *= fs;
	
	float as = (cos(time*.1)*.3);	// there was originally an outer sinh for as and bs but difference is just about 1 percent which doesn't really matter for the camera path
	float ac = sqrt(as*as+1.);
	float bs = (sin(time*.1)*.3);
	float bc = sqrt(bs*bs+1.);
	float cs = sinh((abs(f*2.-1.)-.5)*a);
	float cc = sqrt(cs*cs+1.);
	
	// As last step position & direction are rotated as camera would otherwise fly through an edge instead of a face
	float x = ac*bs;
	float z = ac*bc*cs;
	vec3 pos = vec3(x*a+z,as*e,-x+a*z)/(ac*bc*cc*e);
	//dir = fs;
	dir = normalize(vec3(dir.x*ac*cc-ac*bs*dir.z*cs,-as*dir.z*cs-dir.x*as*bs*cc+dir.y*bc*cc,ac*bc*dir.z)*mat3(a,0,1, 0,e,0, -1.,0,a));

	vec4 hpos = kleinToHyper(pos); // remember position in hyperbolic coordinates
	
	
	float odd = fs;
	
	vec3 color = vec3(0);
	float cremain = 1.;

	float medium = 1.; // 1 = space, -1 = edge
	
	vec4 fa = vec4(39.68454788472584,23.02644300941165,39.78873384183505,27.892411668925842);
	vec4 fb = vec4(29.507160029822894,32.10711839885068,35.17128234256937,26.70192179035261);
	vec4 fc = vec4(24.269388316732734,31.551200069547505,33.74895897903697,38.00825953283422);
	fc.x *= fs0*fs;
	fc.w *= fs0;
	
	
	for (int i = 0; i < 20; i++) {
		float pd = dot(pos,dir);
		float kDist = kleinDist(pos, dir);
		float cremainold = cremain;
		
		float s = dot(fa+fb+fc,vec4(1.));
		vec3 fogcolor = fract((fa.xyz+fb.xyz+fc.xyz)*(fc.yzw+fb.zwx+fa.wxy)+vec3(s))*.8+.2;
		fogcolor *= fogcolor;

		float det = pd*pd-dot(pos,pos)+maxrad2;
		float sDist = (-pd+medium*sqrt(det));
		{sDist = det > 0. && sDist > 0. ? sDist : 1e10;}

		float det3 = pd*pd-dot(pos,pos)+maxrad3;
		float sDist3 = (-pd-medium*sqrt(det3));
		{sDist3 = det3 > 0. && sDist3 > 0. ? sDist3 : 1e10;}

		
		if (sDist < kDist && sDist < sDist3) {
			pos += dir*sDist;
			float eta = refractMed - refractDiff*medium;
			vec3 I = dir;
			vec3 N = -medium*normalize(pos);
			float NI = dot(N,I);
			
			float k = 1.0 - eta * eta * (1.0 - NI*NI);

			
			vec4 hpos2 = kleinToHyper(pos);
			cremain *= exp(-(expFalloffMed + medium*expFalloffDiff)*hyperdist(hpos, hpos2));
			hpos = hpos2;
			color += (cremainold-cremain)*(.3-.3*medium)*fogcolor;
			if (k < 0.0) {
				// reflect instead
				dir = reflect(I, N);
			} else {
				dir = eta * I - (eta * dot(N, I) + sqrt(k)) * N;
				medium = -medium;
			}
		} else if (sDist3 < kDist) {
			pos += dir*sDist3;
			float eta = refractMed - refractDiff*medium;
			vec3 I = dir;
			vec3 N = medium*normalize(pos);
			float NI = dot(N,I);
			
			float k = 1.0 - eta * eta * (1.0 - NI*NI);

			
			vec4 hpos2 = kleinToHyper(pos);
			cremain *= exp(-(expFalloffMed + medium*expFalloffDiff)*hyperdist(hpos, hpos2));
			hpos = hpos2;
			color += (cremainold-cremain)*(.3-.3*medium)*fogcolor;
			if (k < 0.0) {
				// reflect instead
				dir = reflect(I, N);
			} else {
				dir = eta * I - (eta * dot(N, I) + sqrt(k)) * N;
				medium = -medium;
			}
		} else {
			pos += dir*kDist;
			vec4 hpos2 = kleinToHyper(pos);
			cremain *= exp(-(expFalloffMed + medium*expFalloffDiff)*hyperdist(hpos, hpos2));
			hpos = hpos2;
			color += (cremainold-cremain)*(.3-.3*medium)*fogcolor;
			
			dir = hreflect(pos, dir,fa,fb,fc);
			odd = -odd;
		}
	}
	
	gl_FragColor = vec4(sqrt(color), 1.);

}