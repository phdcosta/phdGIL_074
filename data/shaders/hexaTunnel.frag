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

vec3 rotXY(vec3 p, vec2 rad) {
	vec2 s = sin(rad);
	vec2 c = cos(rad);
	
	mat3 m = mat3(
		c.y, 0.0, -s.y,
		-s.x * s.y, c.x, -s.x * c.y,
		c.x * s.y, s.x, c.x * c.y
	);
	return m * p;
}

vec2 repeat(vec2 p, float n) {
	vec2 np = p * n;
	vec2 npfrct = fract(np);
	vec2 npreal = np - npfrct;
	np.x += fract(npreal.y * 0.5);
	
	return fract(np) * 2.0 - 1.0;
}

float hexDistance(vec2 ip) {
	const float SQRT3 = 1.732050807568877;
	const vec2 TRIG30 = vec2(0.5, 0.866025403784439); //x:sine, y:cos
	
	vec2 p = abs(ip * vec2(SQRT3 * 0.5, 0.75));
	float d = dot(p, vec2(-TRIG30.x, TRIG30.y)) - SQRT3 * 0.25;
	
	return (d > 0.0)? min(d, (SQRT3 * 0.5 - p.x)) : min(-d, p.x);
}

float smoothEdge(float edge, float margin, float x) {
	return smoothstep(edge - margin, edge + margin, x);
}

void main(void) {
	const float PI = 3.1415926535;
	vec3 rgb;
	
	vec2 nsc = (gl_FragCoord.xy - resolution * 0.5) / resolution.yy * 2.0;
	vec3 dir = normalize(vec3(nsc, -2.0));
	dir = rotXY(dir, vec2(((mouse.yx / resolution.xy) - 0.5) * PI * 0.35));
	vec2 uv = vec2(atan(dir.y, dir.x) / (PI * 2.0) + 0.5, dir.z / length(dir.xy));
	
	vec2 pos = uv * vec2(1.0, 0.2) - vec2(time * 0.05, time * 0.5);
	
	vec2 p = repeat(pos, 16.0);
	
	float d = hexDistance(p);
	
	float fade = 1.0 / pow(1.0 / length(dir.xy) * 0.3, 2.0);
	fade = clamp(fade, 0.0, 1.0);
	rgb  = mix(vec3(1.0)*fade, vec3(0.0), smoothEdge(0.03, 0.01, d));
	rgb += mix(vec3(1.0, 0.0, 1.0)*fade, vec3(0.0), smoothEdge(0.03, 0.5, d)) * 0.5;
	rgb += mix(vec3(1.0, 0.0, 0.0)*fade, vec3(0.0), smoothEdge(0.03, 1.0, d)) * 0.25;
	
	gl_FragColor = vec4(rgb, 1.0);
}