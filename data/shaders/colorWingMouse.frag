#ifdef GL_ES
precision mediump float;
#endif
// mods by @hintz

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;


void main( void )
{

	vec2 xy = (gl_FragCoord.xy / resolution.xy) * 2.0 / (mouse / resolution.xy);
	
	float x = xy.x - 2.0;
	float y = xy.y - 1.0;
	
	float t = time;
	
	vec3 color = vec3(0.0);
	
	for( float i = 0.0; i < 24.0; i++)
	{
		float yy = y + cos(x*i*0.5 + t+i*0.5) * 0.1;
		x += sin(y*i*0.5 + t+i*0.4321) * 0.1;
		y = yy;
		float value = abs(0.005 / (y*y+x*x));
		color += vec3( value*(i+10.0*sin(x*6.0+i*0.1+t))*0.1, value*i*0.25*cos(x*5.0+i*0.1+t), value*i*sin(x*5.0+i*0.1+t) );
	}
	
	gl_FragColor = vec4(color, 1.0);
}