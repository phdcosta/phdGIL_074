#ifdef GL_ES
precision mediump float;
#endif

// Posted by Trisomie21 : 2D noise experiment (pan/zoom)

uniform float time;
uniform vec2 resolution;
uniform vec2 mouse;

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

vec3 sun( vec2 pos ) {
	vec2 p = mouse.xy / resolution.xy;
	return vec3(0.5, 1.5, 4.0) / (distance( p, pos ) * 15.0);
}

void main( void ) {
	vec2 p = gl_FragCoord.xy / resolution.xy;
	p *= (sin(time*.1+10.)+1.4)*.05;
	p += 1.5;
	vec3 c = vec3(.0, .0, .1);
	c = mix(c, vec3(.7, .1, .1), pow(cloud(p*.20+time*.0002), 3.)*2.);
	c = mix(c, vec3(.7, .8, 1.), pow(cloud(p*.10+time*.0002), 3.));
	c = mix(c, vec3(1., 1., 1.), pow(cloud(p*.05+time*.0002), 2.)*1.2);
	c = sun(gl_FragCoord.xy / resolution.xy) + c / 1.0;
	gl_FragColor = vec4(c, 1.);
}