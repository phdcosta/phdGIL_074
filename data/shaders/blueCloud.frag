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

vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec2 mod289(vec2 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x) {
  return mod289(((x*34.0)+1.0)*x);
}

float snoise(vec2 v) {
  const vec4 C = vec4(0.211324865405187, 0.366025403784439, -0.577350269189626, 0.024390243902439);
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);
  vec2 i1;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;
  i = mod289(i);
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 )) + i.x + vec3(0.0, i1.x, 1.0 ));
  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;
  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}

vec4 perlin( void ) {
	vec2 p = ( gl_FragCoord.xy / resolution.xy );
	p = (p * 2.0 - 1.0) * vec2(800, 600);
	p.y -= time * 596.0;
	float z = 84.0;
	float speed = 10.0;
	float v = snoise(p / 128. + z + 0.05 * time * speed);  p += 1017.0;
	v +=  snoise(p / 64. + z + 0.05 * time * speed) /2.;
	v +=  snoise(p / 32. + z + 0.05 * time * speed) /4.;
	v +=  snoise(p / 16. + z + 0.05 * time * speed) /8.;
	v +=  snoise(p / 8. + z + 0.05 * time * speed) /16.;
	v +=  snoise(p / 4.+ z + 0.05 * time * speed) /32.;
	v +=  snoise(p / 2.+ z + 0.05 * time * speed) /64.;
	v +=  snoise(p + z + 0.05 * time * speed) /128.;
	v = 0.1 * v + 0.5;
	return vec4(v*1.,0.5 + v,0.5 + v,0.5 + v);
}

void main() {
	vec2 position = gl_FragCoord.xy / resolution.xy - mouse.xy / resolution.xy;
	position.x *= (resolution.x / resolution.y);
	vec2 p = position * 800.;
	float len = sqrt(p.x*p.x+p.y*p.y*2.0);
	float ang = 2.0*atan(p.y,(len+p.x));
	ang += pow(len, 0.5)*1.0;
	ang = 1.;
	float f = ang + -time*3.141592*2.0;
	f = ang + 3.141592*2.0;
	float r = 2.0 - sin(f) * 0.5;
	float g = 3.0 - sin(f) * 0.5;
	float b = 8.0 - sin(f) * 0.5;
	vec3 color = vec3(r, g, b);
	color /= 2.;
	float ds = len/3500. * perlin().y;
	ds *= 10.;
	color *= (1.0-ds);
	color -= vec3(0.5);
	color *= vec3(1.5);
	color += vec3(0.5);
	gl_FragColor = clampColor( color, clrLow, clrHigh );
}
