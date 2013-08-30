#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

float hash( float n ) { return fract(sin(n)*43758.5453); }

float star(vec2 position) {
	float col = (pow(tan(position.x) * tan(position.y),2.0) * 1000000000000.0) * length(position);
	return clamp(0.8 - col, 0.0, 0.9) * step(length(position), 0.02);
}

void main( void ) {
	
	vec2 position = ( gl_FragCoord.xy / resolution.xx ) - vec2(0.5);
	
	float color = 0.0;
	
	for(float i = 0.0; i < 100.0; i++) {
		float xo = hash(i) - 0.5;
		float yo = hash(i+100.0) - 0.5;
		float so = (cos(time * 2.0 + hash(i*120.0)*100.0) + 1.2) / 2.0;
		color += star(position + vec2(xo,yo) ) * so;
	}
	vec3 stars = smoothstep(0.5, 1.0, color) * vec3(1.0) + smoothstep(0.0, 0.5, color) * vec3(0.0, 0.0, 0.3);
	gl_FragColor = vec4(stars, 1.0);

}