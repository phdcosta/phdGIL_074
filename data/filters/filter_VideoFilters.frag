//Metamaterial
//https://www.shadertoy.com/view/lssGz8

#ifdef GL_ES
precision highp float;
#endif

uniform float alpha;

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

uniform sampler2DRect tex0;
uniform vec2 size0;

uniform sampler2DRect tex1;
uniform vec2 size1;

vec2 zoom0 = vec2(2., 2.);

void main(void)
{
    vec2 p = gl_FragCoord.xy/size0.xy;
    
	vec4 col = texture2DRect(tex0, p);	
	
	//Desaturate
    if(p.x<.25)
	{
		col = texture2DRect(tex0, p*size0);
		col = vec4( (col.r+col.g+col.b)/3. );
	}
	//Chromatic aberration
	else if (p.x<.5)
	{
		vec2 offset = vec2(.05,.0);
		col.r = texture2DRect(tex0, (p+offset.xy)*size0).r;
		col.g = texture2DRect(tex0, (p          )*size0).g;
		col.b = texture2DRect(tex0, (p+offset.yx)*size0).b;
	}
	//Invert
	else if (p.x<.75)
	{
		col = vec4(1.) - texture2DRect(tex0, p*size0);
	}
	//Color switching
	else
	{
		col.rgb = texture2DRect(tex0, p*size0).gbr;
	}
	
	
	//Line
	if( (p.x<.251&&p.x>.249) || (p.x<.501&&p.x>.499) ||  (p.x<.751&&p.x>.749) )
		col = vec4(1.);	
	
    gl_FragColor = col;
}