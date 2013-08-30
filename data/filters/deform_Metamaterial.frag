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

#define pi 3.141592653589793238462643383279

float atan2(float y, float x){
	if(x>0.) return atan(y/x);
	if(y>=0. && x<0.) return atan(y/x) + pi; 
	if(y<0. && x<0.) return atan(y/x) - pi; 
	if(y>0. && x==0.) return pi/2.;
	if(y<0. && x==0.) return -pi/2.;
	if(y==0. && x==0.) return pi/2.; // undefined usually
	return pi/2.;
}

vec2 uv_polar(vec2 uv, vec2 center){
	vec2 c = uv - center;
	float rad = length(c);
	float ang = atan2(c.x,c.y);
	return vec2(ang, rad);
}

vec2 uv_lens_half_sphere(vec2 uv, vec2 position, float radius, float refractivity){
	vec2 polar = uv_polar(uv, position);
	float cone = clamp(1.-polar.y/radius, 0., 1.);
	float halfsphere = sqrt(1.-pow(cone-1.,2.));
	float w = atan2(1.-cone, halfsphere);
	float refrac_w = w-asin(sin(w)/refractivity);
	float refrac_d = 1.-cone - sin(refrac_w)*halfsphere/cos(refrac_w);
	vec2 refrac_uv = position + vec2(sin(polar.x),cos(polar.x))*refrac_d*radius;
	return mix(uv, refrac_uv, float(length(uv-position)<radius));
}

vec2 wrap_flip(vec2 uv){
	return vec2(1.)-abs(fract(uv*.5)*2.-1.);
}
 
float border(vec2 domain, float thickness){
   vec2 uv = fract(domain-vec2(0.5));
   uv = min(uv,1.-uv)*2.;
   return clamp(max(uv.x,uv.y)-1.+thickness,0.,1.)/(thickness);
}

void main(void)
{
	// domain map
	vec2 uv = gl_FragCoord.xy / size0.xy;
	
	// aspect-ratio correction
	vec2 aspect = vec2(1.,size0.y/size0.x);
	vec2 uv_correct = 0.5 + (uv -0.5)* aspect;
	vec2 mouse_correct = 0.5 + ( mouse.xy / size0.xy - 0.5) * aspect;

	vec2 pos = vec2(0.5);
	//pos = mouse_correct;
	
	vec2 uv_lens_distorted = uv_lens_half_sphere(uv_correct, pos, 0.6, -0.25);
	
	uv_lens_distorted = 0.5 + (uv_lens_distorted - 0.5) / aspect;
	
	uv_lens_distorted = wrap_flip(uv_lens_distorted);
	
	gl_FragColor = texture2DRect(tex0, (uv_lens_distorted) * (1.-border(uv_lens_distorted, 0.01)*0.1) * size0);

}