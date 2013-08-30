#version 120

uniform float time;
uniform float alpha;
uniform vec2 mouse;
uniform vec2 resolution;
uniform vec4 clrBase;
uniform vec4 clrLow;
uniform vec4 clrHigh;
uniform vec4 params;
uniform vec2 pos;
uniform vec2 scale;
uniform sampler2DRect tex0;

vec2 c = pos.xy;

float pi = 3.14159;

vec3 hsv2rgb(float h,float s,float v) {
	return mix(vec3(1.),clamp((abs(fract(h+vec3(3.,2.,1.)/3.)*6.-3.)-1.),0.,1.),s)*v;
}

vec3 shader( vec2 p ) {
	vec3 color = vec3(0.0);
	vec2 uv = vec2(0.0);
	float a = atan((p.x-c.x)/(p.y-c.y));
	float d = sqrt((p.x-c.x)*(p.x-c.x)+(p.y-c.y)*(p.y-c.y));
	uv = vec2(mod(a*8.0+sin(d*8.0),pi*2.0)/pi*0.5,mod(d*4.0,1.0));
	float d2 = sqrt((uv.x-c.x)*(uv.x-c.x)+(uv.y-c.y)*(uv.y-c.y))*1.5;
	color = mix(vec3(0.0),hsv2rgb(d+time*0.5,1.0,1.0),d2);
	color = mix(color,vec3(1.0),pow(d2,4.0));
	return vec3( color);
}

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

void main( void ) {
	vec2 p = ( gl_FragCoord.xy / resolution.xy );
	vec3 color = vec3(0.0);
	vec2 uv = vec2(0.0);
	float a = atan((p.x-c.x)/(p.y-c.y));
	float d = sqrt((p.x-c.x)*(p.x-c.x)+(p.y-c.y)*(p.y-c.y));
	d = sin(d*4.0*params.x+time)*d;
	uv = vec2(mod(a*8.0*params.y+sin(d*16.0*params.z+time),pi*2.0)/pi*0.5,mod(pow(d*8.0*params.a,0.75),1.0));
	d = sqrt((uv.x-c.x)*(uv.x-c.x)+(uv.y-c.y)*(uv.y-c.y));
	color = shader(uv);
	gl_FragColor = clampColor(color, clrLow, clrHigh);
}
