//Mod Filter
//https://www.shadertoy.com/view/4sfGR4

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

float pi = 3.1415927410125;

float MAXD=1000.0;
int rm_Iterations =100;
float perspective=1.25,zoom=1.2;
vec3 cameraLocation=vec3(0.0, -12.5, -24.6);
vec3 light=vec3(-0.1,-0.1, -1.);
float rotX=1.5;
float dist=.95;
vec3 center=vec3(0.0,0.0,0.0);
vec3 shift=vec3(64./2.,0.,36./1.5);

vec3 rotateX( const in vec3 p, const in float angle ){
	float s = sin(angle);
	float c = cos(angle);	
	return vec3( p.x, c * p.y + s * p.z, -s * p.y + c * p.z);
}

float map( vec3 p ){
	p=rotateX(p,rotX);//a little tilted for mo' 3d effect
	p +=shift;
	if (p.x>-0.5 && p.x<64. && p.z>0. && p.z<48.){
	vec4 tex=texture2DRect(tex0,p.xz/vec2(64.0,48.0).xy*size0);

	float tl=max(.3,tex.r*1.5);//small size vs. big size
	float div=2.;
    p.x = mod( p.x + dist/div, dist ) - dist/div;
    //float dist2=2.75;
    //p.y = (mod( p.y + dist2/div, dist2 ) - dist2/div)-1.5;
   	p.z = mod( p.z + dist/div, dist ) - dist/div;

    	return length( center - p ) - tl;
    	}
    	else return float(1.);
}


vec3 calc_normal(vec3 v){
  	float e=0.0001;
  	vec3 n=vec3(
    map(vec3(v.x+e,v.y,v.z))-map(vec3(v.x-e,v.y,v.z)),
    map(vec3(v.x,v.y+e,v.z))-map(vec3(v.x,v.y-e,v.z)),
    map(vec3(v.x,v.y,v.z+e))-map(vec3(v.x,v.y,v.z-e)));
  	return normalize(n);
}

float ao(vec3 p, vec3 n, float d){
  	float o=1.0,ii=5.0;
  	for(int i=0;i<5;i++){
    	vec3 tmpV=p+n*(ii*d);
    	float tmp=map(tmpV);
    	if(tmp<0.0) tmp=0.0;
    	o-=(ii*d-tmp)/pow(2.0,ii);
    	ii=ii-1.0;
  	}
  	return o;
}

void main(void){
	float x,y,off=0.0;
	vec4 col;
  	float rx,ry,d;
  	int steps=0;
  	vec3 ray,direction;

  	vec2 p = 2.0*gl_FragCoord.xy;
  	p.x/= resolution.x;
  	p.y/= resolution.x;
  	p-=1.0;
  	p.y=-p.y;
	p=p/zoom;
 	ray=vec3(p.x,p.y,0);
 	ray=ray+cameraLocation;
  	direction=vec3(p.x*perspective,p.y*perspective,1.0);
  	direction=normalize(direction);
  	col=vec4(0.25+(p.y+0.5)/3.0,0.25+(p.y+0.5)/3.0,0.33+(p.y+0.5)/3.0,1.0);

  	for(int i=0;i<100;i++){
		d=map(ray);
      if(d>=MAXD){
         // Infinite distance
          break;
      }
      if(d<0.00001){
      	vec3 n=calc_normal(ray);
      	//float normlight=0.25*dot(light,n);
		float normlight=0.25*max(0.,dot(light,n));//thx @ las
      	float aolight=ao(ray,n,0.25);//float aolight=ao(ray,n,0.25);
      	if(normlight<0.0) normlight=0.0;
        	float ambient=0.75;
      	float c=(normlight+ambient)*aolight;
      	col=vec4(c,c,c,1.0);
		vec3 p2=rotateX(ray,rotX);
		p2 +=shift;
		vec4 tex=texture2DRect(tex0,p2.xz/vec2(64.0,48.0).xy*size0);
		col *=tex /(1.+map(ray));//col +=vec3(1.,0.1,0.1) /(1.+map(ray));//thanks @ XT95
        	break;
      }
      ray+=direction*d;
	}
	
	gl_FragColor = col;
}