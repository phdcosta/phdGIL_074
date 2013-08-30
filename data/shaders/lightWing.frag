#ifdef GL_ES
precision mediump float;
#endif
// mods by dist

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;


void main( void )
{

	vec2 uPos = ( gl_FragCoord.xy / resolution.xy );//normalize wrt y axis
	//suPos -= vec2((resolution.x/resolution.y)/2.0, 0.0);//shift origin to center
	
	uPos.x -= 0.0;
	uPos.y -= 0.5;
	
	vec3 color = vec3(0.0);
	float vertColor = 0.0;
	for( float i = 0.0; i < 10.0; ++i )
	{
		float t = time * 4.0;
	
		uPos.y += sin( uPos.x*(i+5.0) + t+i/15.0 ) * 0.048;
		float fTemp = abs(1.0 / uPos.y / 120.0);
		vertColor += fTemp;
		color += vec3( fTemp*(10.0-i)/10.0, fTemp*i/10.0, pow(fTemp,0.99)*1.5 );
	}
	
	vec4 color_final = vec4(color, 1.0);
	gl_FragColor = color_final;
}