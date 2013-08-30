// CONVOLUTION

#ifdef GL_ES
precision highp float;
#endif

#define KERNELSIZE 9

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
uniform sampler2DRect tex0;
uniform vec2 size0;
uniform sampler2DRect tex1;
uniform vec2 size1;
uniform float alpha;

vec2 zoom0 = vec2(2., 2.);
float pi = 3.1415927410125;

float kernel[KERNELSIZE];
vec2 offset[KERNELSIZE];

void main( void ) {

kernel[0] = 1.0/6.0;
kernel[1] = 1.0/6.0;
kernel[2] = 1.0/6.0;
kernel[3] = 0.0;
kernel[4] = 0.0;
kernel[5] = 0.0;
kernel[6] = 1.0/6.0;
kernel[7] = 1.0/6.0;
kernel[8] = 1.0/6.0;

offset[0] = vec2(-1.0, -1.0);
offset[1] = vec2(0.0, -1.0);
offset[2] = vec2(1.0, -1.0);
offset[3] = vec2(-1.0,  0.0);
offset[4] = vec2(0.0, 0.0);
offset[5] = vec2(1.0,  0.0);
offset[6] = vec2(-1.0,  1.0);
offset[7] = vec2(0.0, 1.0);
offset[8] = vec2(1.0,  1.0);

vec2 p = -1.0 + 2.0 * gl_TexCoord[0].st / size0 + vec2(.0,.0);
int i = 0;
vec4 sum = vec4(0.0);
for( i = 0; i < KERNELSIZE; i++ ) {
	vec4 tmp = texture2DRect(tex0, gl_TexCoord[0].st + offset[i]);
	sum += tmp * kernel[i];
	sum.a = 1.0;
	}
   	gl_FragColor = sum;
}

