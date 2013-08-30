precision mediump float;

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
varying vec2 surfacePosition;

void main( void ) {
	vec2 p = 1.0 - 2.0 * gl_FragCoord.xy / resolution.xy; p *= mouse.x / resolution.x * 5.0;
	float speed = 0.25;
	vec3 color = vec3(0.5,0.5,.75);
	vec2 loc = vec2(
		cos(time/4.0*speed)/1.9-cos(time/2.0*speed)/3.8,
		sin(time/4.0*speed)/1.9-sin(time/2.0*speed)/3.8
	);
	float depth;
	for(int i = 0; i < 50; i+=1){
		p = vec2(p.x*p.x-p.y*p.y, 2.0*p.x*p.y)+loc;
		depth = float(i);
		if((p.x*p.x+p.y*p.y) >= 4.0) break;
	}
	gl_FragColor = vec4(clamp(color*depth*0.05, 0.1, 1.0), 1.0 );
}