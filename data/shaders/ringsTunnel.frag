#version 120

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

void main() {
	vec2 pos = gl_FragCoord.xy / resolution.xy - pos.xy;
	pos.x *= (resolution.x / resolution.y);
	float u = length(pos);
	float v = atan(pos.y, pos.x);
	float t = time / 5.0 *params.x + 1.0 / u;
	float intensity = abs(sin(t * 10.0 *params.y + v)) * .25 * u * 0.25;
	vec3 col = vec3(abs(sin(time)), abs(sin(time + 3.1415927410125 / 3.0)), abs(sin(time - 3.1415927410125 / 3.0))) * 16.0 * params.z;
	gl_FragColor = clampColor(col * intensity, clrLow, clrHigh);
}
