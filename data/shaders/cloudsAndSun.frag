// little fluffy clouds
// @simesgreen
// coloring and cloud mod by @rotwang, thank you for these great clouds. 

#ifdef GL_ES
precision highp float;
#endif

uniform vec2 resolution;
uniform float time;
uniform vec2 mouse;

const int _VolumeSteps = 64;
const float _StepSize = 0.05; 
const float _Density = 0.1;

const float _SphereRadius = 1.2;
const float _NoiseFreq = 0.5;
const float _NoiseAmp = 2.0;

// Description : Array and textureless GLSL 2D/3D/4D simplex 
//               noise functions.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise
// 

vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 mod289(vec4 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x) {
     return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r)
{
  return 1.79284291400159 - 0.85373472095314 * r;
}

float snoise(vec3 v)
  { 
  const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
  const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

  // First corner
  vec3 i  = floor(v + dot(v, C.yyy) );
  vec3 x0 =   v - i + dot(i, C.xxx) ;

  // Other corners
  vec3 g = step(x0.yzx, x0.xyz);	  
  vec3 l = 1.0 - g;
  vec3 i1 = min( g.xyz, l.zxy );
  vec3 i2 = max( g.xyz, l.zxy );

  //   x0 = x0 - 0.0 + 0.0 * C.xxx;
  //   x1 = x0 - i1  + 1.0 * C.xxx;
  //   x2 = x0 - i2  + 2.0 * C.xxx;
  //   x3 = x0 - 1.0 + 3.0 * C.xxx;
  vec3 x1 = x0 - i1 + C.xxx;
  vec3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y
  vec3 x3 = x0 - D.yyy;      // -1.0+3.0*C.x = -0.5 = -D.y

  // Permutations
  i = mod289(i); 
  vec4 p = permute( permute( permute( 
             i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
           + i.y + vec4(0.0, i1.y, i2.y, 1.0 )) 
           + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

  // Gradients: 7x7 points over a square, mapped onto an octahedron.
  // The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
  float n_ = 0.142857142857; // 1.0/7.0
  vec3  ns = n_ * D.wyz - D.xzx;

  vec4 j = p - 49.0 * floor(p * ns.z * ns.z);  //  mod(p,7*7)

  vec4 x_ = floor(j * ns.z);
  vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

  vec4 x = x_ *ns.x + ns.yyyy;
  vec4 y = y_ *ns.x + ns.yyyy;
  vec4 h = 1.0 - abs(x) - abs(y);

  vec4 b0 = vec4( x.xy, y.xy );
  vec4 b1 = vec4( x.zw, y.zw );

  //vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
  //vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
  vec4 s0 = floor(b0)*2.0 + 1.0;
  vec4 s1 = floor(b1)*2.0 + 1.0;
  vec4 sh = -step(h, vec4(0.0));

  vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
  vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

  vec3 p0 = vec3(a0.xy,h.x);
  vec3 p1 = vec3(a0.zw,h.y);
  vec3 p2 = vec3(a1.xy,h.z);
  vec3 p3 = vec3(a1.zw,h.w);

  //Normalise gradients
  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;

  // Mix final noise value
  vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
  m = m * m;
  return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), 
                                dot(p2,x2), dot(p3,x3) ) );
}


float fbm(vec3 p)
{
    float f;
    f = 0.5000*snoise( p ); p = p*2.02;
    f += 0.2500*snoise( p ); p = p*2.03;
    f += 0.1250*snoise( p ); p = p*2.01;
    f += 0.0625*snoise( p );
    return f;
}

float fbm2(vec3 p)
{
    const int octaves = 4;
    float amp = 0.5;
    float freq = 1.0;
    float n = 0.0;	
    for(int i=0; i<octaves; i++) {
        n += snoise(p*freq)*amp;
	freq *= 2.1;
	amp *= 0.5;
    }
    return n;
}

// returns signed distance to surface
float distanceFunc(vec3 p)
{	
	p.y -= 2.0; // move up
	p.z += time;
	//p += snoise(p*0.1)*2.0;	// domain warp		
	
	// repeat on grid
	vec3 q = p;
	q.xz = mod(q.xz - vec2(2.5), 5.0) - vec2(2.5);
	
        q.y *= 2.0;	// squash in y
	
	float d = length(q) - _SphereRadius;	// distance to sphere

	// offset distance with noise
	//p = normalize(p) * _SphereRadius;	// project noise point to sphere surface
	p.y += time*0.3;
	d += fbm(p*_NoiseFreq) * _NoiseAmp;
	return d;
}

// map distance to color
vec4 shade(float d)
{	
	return mix(vec4(0.7, 0.7, 0.7, _Density),
		   vec4(0.5, 0.5, 0.5, 0), smoothstep(0.5, 1.0, d));
}

// maps position to color
vec4 volumeFunc(vec3 p)
{
	float d = distanceFunc(p);
	vec4 c = shade(d);
	c.rgb *= smoothstep(1.0, 2.0, p.y)*0.5+0.5;
	float r = length(p)*0.04 + sin(time/4.0)*0.2;
	c.a *= exp(-r*r)*0.25;	// fog
	return c;
}

vec3 sky(vec3 v)
{
	const vec3 sunDir = vec3(0.577, 0.577, 0.577);
	vec3 c = mix(vec3(0.1, 0.1, 0.1), vec3(0.1, 0.25, 0.75), abs(v.y*1.75)-0.5);
	float sun = pow(dot(v, sunDir), 100.0)*0.45;
	c += sun*vec3(3.0, 2.0, 0.5);
	return c;
}

// ray march volume
vec4 rayMarch(vec3 rayOrigin, vec3 rayStep, vec4 sum, out vec3 pos)
{
	pos = rayOrigin;
	for(int i=0; i<_VolumeSteps; i++) {
		vec4 col = volumeFunc(pos);
#if 1
		sum = mix(sum, col, col.w);	// under operator for back-to-front
#else	
		col.rgb *= col.a;		// pre-multiply alpha
		sum = sum + col*(1.0 - sum.a);	// over operator for front-to-back
#endif
			
#if 0		
		// exit early if opaque
        	if (sum.a > _OpacityThreshold)
            		break;
#endif		
		pos += rayStep;
		//rayStep *= 1.01;
	}
	return sum;
}

float intersectPlane(vec3 ro, vec3 rd, vec4 plane)
{
    float t = (-plane.w - dot(ro, plane.xyz)) / dot(rd, plane.xyz);
    return t;
}

void main(void)
{
    vec2 p = (gl_FragCoord.xy / resolution.xy)*2.0-1.0;
    p.x *= resolution.x/resolution.y;
	
    float rotx = 4.0; // mouse.y*1.0 + 16.0;
    float roty = 4.0; // - mouse.x*4.0;

    //float zoom = 5.0*surfaceSize.y;
    float zoom = 1.0;

    // camera
    vec3 ro = zoom*normalize(vec3(cos(roty), cos(rotx), sin(roty)));
	
    vec3 ww = normalize(vec3(0.0,0.0,0.0) - ro);
    vec3 uu = normalize(cross( vec3(0.0,1.0,0.0), ww ));
    vec3 vv = normalize(cross(ww, uu));
    vec3 rd = normalize( p.x*uu + p.y*vv + 1.5*ww );
	
    //ro += rd*4.0;
    //ro += rd*20.0;
    bool hit = true;
    float t1 = intersectPlane(ro, rd, vec4(0.0, -1.0, 0.0, 3.0));
    float t2 = intersectPlane(ro, rd, vec4(0.0, -1.0, 0.0, 1.0));
    float tnear = min(t1, t2);
    float tfar = max(t1, t2);
    tnear = max(tnear, 0.0);
    tfar = max(tfar, 0.0);		
    vec3 pnear = ro+rd*tnear;
    vec3 pfar = ro+rd*tfar;	
	
    ro = pfar;
    float stepSize = length(pfar - pnear) / float(_VolumeSteps);
    rd = -rd;
	
    vec3 hitPos;
    //vec4 col = vec4(0, 0.25, 0.5, 0);
    vec4 col = vec4(sky(rd), 0);
    if (hit) {
	    //col = rayMarch(ro, rd*_StepSize, col, hitPos);
	    col = rayMarch(ro, rd*stepSize, col, hitPos);
    }

    //col *= smoothstep(4.0, 0.7, dot(p, p));
	
    gl_FragColor = vec4(col.rgb, 1.0);
}