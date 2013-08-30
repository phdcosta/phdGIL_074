#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 resolution;

vec4 textureRND2D(vec2 uv){
	uv = floor(fract(uv)*1e3);
	float v = uv.x+uv.y*1e3;
	return fract(1e5*sin(vec4(v*1e-2, (v+1.)*1e-2, (v+1e3)*1e-2, (v+1e3+1.)*1e-2)));
}

float noise(vec2 p) {
	vec2 f = fract(p*1e3);
	vec4 r = textureRND2D(p);
	f = f*f*(3.0-2.0*f);
	return (mix(mix(r.x, r.y, f.x), mix(r.z, r.w, f.x), f.y));	
}

float cloud(vec2 p) {
	float v = 0.0;
	v += noise(p*1.)*.5000;
	v += noise(p*2.)*.2500;
	v += noise(p*4.)*.1250;
	v += noise(p*8.)*.0625;
	v += noise(p*16.)*.03125;
	return v;
}

vec3 func( vec2  p) {
	p = p*.1+.5;
	vec3 c = vec3(.0, .0, .1);
	vec2 d = vec2(time*.0005, 0.);
	c = mix(c, vec3(.7, .1, .1), pow(cloud(p*.20+d), 3.)*2.);
	c = mix(c, vec3(.7, .8, 1.), pow(cloud(p.y*p*.10+d), 3.));
	c = mix(c, vec3(1., 1., 1.), pow(cloud(p.y*p*.05+d), 1.5)*1.);
	return c;
}

void main( void ) {
	vec2 p = 1.0 - 2.0 * gl_FragCoord.xy / resolution.xy; p.y *= resolution.y / resolution.x; p *= 1.25;
	float d = length(p);
	
	float a = acos(d) - 3.141592 / 2.0;
	p = vec2(a * p.x / d, a * p.y / d);
	
	gl_FragColor = vec4(func(p)*max(1.-d*d*d, 0.), 1.0);
}