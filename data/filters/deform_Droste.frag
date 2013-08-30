#version 120

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

const float TWO_PI = 3.141592*2.0;

//ADJUSTABLE PARAMETERS:
const float branches = 1.0;
const float scale = 0.125;
const float speed = 0.10;

//Complex Math:
vec2 complexExp(in vec2 z){
	return vec2(exp(z.x)*cos(z.y),exp(z.x)*sin(z.y));
}
vec2 complexLog(in vec2 z){
	return vec2(log(length(z)), atan(z.y, z.x));
}
vec2 complexMult(in vec2 a,in vec2 b){
	return vec2(a.x*b.x - a.y*b.y, a.x*b.y + a.y*b.x);
}
float complexMag(in vec2 z){
	return float(pow(length(z), 2.0));
}
vec2 complexReciprocal(in vec2 z){
	return vec2(z.x / complexMag(z), -z.y / complexMag(z));
}
vec2 complexDiv(in vec2 a,in vec2 b){
	return complexMult(a, complexReciprocal(b));
}
vec2 complexPower(in vec2 a, in vec2 b){
	return complexExp( complexMult(b,complexLog(a))  );
}
//Misc Functions:
float nearestPower(in float a, in float base){
	return pow(base,  ceil(  log(abs(a))/log(base)  )-1.0 );
}
float map(float value, float istart, float istop, float ostart, float ostop) {
	   return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

vec4 droste(in vec2 co){
		
	//SHIFT AND SCALE COORDINATES TO <-1,1>
	vec2 z = (co-0.5)*2.0;
	
	//ESCHER GRID TRANSFORM:
	float factor = pow(1.0/scale,branches);
	z = complexPower(z, complexDiv(vec2( log(factor) ,TWO_PI), vec2(0.0,TWO_PI) ) ); 
	
	//RECTANGULAR DROSTE EFFECT:
	//z *= 1.0+fract(time*speed)*(scale-1.0);
	z *= 1.0+fract(time*speed)*(scale-1.0);
	float npower = max(nearestPower(z.x,scale),nearestPower(z.y,scale));
	z.x = map(z.x,-npower,npower,-1.0,1.0);
	z.y = map(z.y,-npower,npower,-1.0,1.0);
	
	//UNDO SHIFT AND SCALE:
	z = z*0.5+0.5;
		
	return texture2DRect(tex0, z * size0);
}

void main( void ) {

	vec2 p = -1.0 + 2.0 * gl_TexCoord[0].st / size0 + vec2(.0,.0);

	vec2 s = gl_FragCoord.xy / resolution.xy;

	gl_FragColor = droste(s);//+vec2(mouse.x,-1.0*mouse.y));
}
