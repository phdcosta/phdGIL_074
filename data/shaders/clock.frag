#ifdef GL_ES
precision mediump float;
#endif

#define PI 3.14159
#define HPI (3.14159/2.)

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

//from http://geomalgorithms.com/a02-_lines.html
float dist_Point_to_Segment(vec2 P, vec2 P0, vec2 P1)
{
     vec2 v = P1 - P0;
     vec2 w = P - P0;
     float c1 = dot(w,v);
     if ( c1 <= 0. )
          return length(P-P0);
     float c2 = dot(v,v);
     if ( c2 <= c1 )
          return length(P-P1);
     float b = c1 / c2;
     vec2 Pb = P0 + b * v;
     return length(P-Pb);
}

void main( void ) {

	vec2 p = (gl_FragCoord.xy);
	
	float aspfix = (800./resolution.y);
	
	float sr = 256./aspfix,
	      mr = 192./aspfix,
	      hr = 128./aspfix,
	      or = 272./aspfix,
	      lr = 16./aspfix;
	
	float s = floor(time*35.)/9.65; //Just a guess trying to get seconds from time.
	
	float m = s/60.;
	
	float h = m/60.;
	
	vec2 s_cossin = vec2(cos(-s+HPI),sin(-s+HPI))*sr+ (resolution/2.);
	vec2 m_cossin = vec2(cos(-m+HPI),sin(-m+HPI))*mr+ (resolution/2.);
	vec2 h_cossin = vec2(cos(-h+HPI),sin(-h+HPI))*hr+ (resolution/2.);
	
	vec2 epos = s_cossin ;
	
	float dist = dist_Point_to_Segment(p,resolution/2.,s_cossin);
	
	dist = min(dist,dist_Point_to_Segment(p,resolution/2.,m_cossin));
	
	dist = min(dist,dist_Point_to_Segment(p,resolution/2.,h_cossin));
	
	dist = min(dist,or-distance(resolution/2.,p));
	
	float bright = ((1.-pow(dist/lr,0.95)));
		
	gl_FragColor = vec4( vec3(1.,0.5,0.)*bright+bright, 1.0 );
}