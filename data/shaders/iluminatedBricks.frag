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

const mat2 m = mat2( 0.80,  0.60, -0.60,  0.80 );
float hash( float n ) {
	return fract(sin(n)*43758.5453);
}

float noise( in vec2 x ) {
	vec2 p = floor(x);
	vec2 f = fract(x);
	f = f*f*(3.0-2.0*f);
	float n = p.x + p.y*57.0;
	float res = mix(mix( hash(n+0.0), hash(n+1.0),f.x), mix( hash(n+57.0), hash(n+58.0),f.x),f.y);
	return res;
}

float fbm( vec2 p ) {
	float f = 0.0;
	f += 0.50000*noise( p ); p = m*p*2.02;
	f += 0.25000*noise( p ); p = m*p*2.03;
	f += 0.12500*noise( p ); p = m*p*2.01;
	f += 0.06250*noise( p ); p = m*p*2.04;
	f += 0.03125*noise( p );
	return f/0.984375;
}

vec4 heightmap(vec2 position) {
	vec2 bricksize = vec2(resolution.x/14.,resolution.y/16.).xy / resolution.xy;
	vec2 brickspace = vec2(bricksize.x/8.,bricksize.y/8.);
	float height = 0.0;
	vec3 color = vec3(0.95,0.75,0.5);
	float row = floor(position.y/bricksize.y);
	if (mod(row, 2.0) < 1.0) position.x += bricksize.x/2.;
	height = min(fbm(position*0.125)-0.45,0.0);
	color *= fbm(position*0.0625)*0.5+0.75;
	if(mod(position.x,bricksize.x) < brickspace.x || mod(position.y,bricksize.y) < brickspace.y) {
		height = -2.0/resolution.x+fbm(position)*.5;
		color = vec3(fbm(position)*0.75);
	}
	return vec4(color,height);
}

float n1,n2,n3,n4;
vec2 size = vec2(-0.4,0.0);
void main( void ) {
	vec2 pos = gl_FragCoord.xy / resolution.xy;
	vec2 mou = mouse.xy / resolution.xy;
	vec2 off = vec2(pos.x,pos.y-1.0/resolution.y);
	vec3 col = heightmap(pos).xyz;
	n1 = heightmap(off).w;
	off = vec2(pos.x-1.0/resolution.x,pos.y);
	n2 = heightmap(off).w;
	off = vec2(pos.x+1.0/resolution.x,pos.y);
	n3 = heightmap(off).w;
	off = vec2(pos.x,pos.y+1.0/resolution.y);
	n4 = heightmap(off).w;
	vec3 va = normalize(vec3(size.xy,n2-n3));
	vec3 vb = normalize(vec3(size.yx,n1-n4));
	vec3 p2m = vec3(-((pos)-mou),64.0/resolution.y);
	vec3 normal = vec3(cross(va,vb));
	float color = dot(normal.xyz, normalize(p2m))*.5+.5;
	vec3 colorvec = vec3(pow(color,1.0),pow(color,1.0),pow(color,1.0));
	float brightness = 1./sqrt(1.5+pow(distance(mou,pos)*4.,3.));
	gl_FragColor = clampColor( col+((colorvec*brightness)-0.5)*2.0, clrLow, clrHigh );
}
