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

#ifdef GL_ES
precision mediump float;
#endif

varying vec3 normal, lightDir;
varying vec4 color;

void main()
{
        float intensity;
        float factor;
        vec3 n;

        n = normalize(normal);
        intensity = max(dot(lightDir,n),0.0);

        if (intensity > 0.95)
                factor = 1.0;
        else if (intensity > 0.5)
                factor = 0.5;
        else if (intensity > 0.25)
                factor = 0.25;
        else
                factor = 0.10;
        gl_FragColor = color * vec4(factor, factor, factor, 1);
}