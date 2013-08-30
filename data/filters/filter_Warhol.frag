// numTiles resp. stride, dotsize als Parameter

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

const float steps = 2.0;
const float dotsize = 1.0 / steps ;
const float half_step = dotsize / 2.0;

vec2 texCoord = vec2(0., 0.);

// This should be a static 2x2 pixel texture
// but it's done as an if statement instead ...

/*
mat4x4 tint = mat4(
                 vec4(1.0,0.0,1.0,0.0),
                 vec4(1.0,0.0,1.0,0.0),
                 vec4(1.0,0.0,1.0,0.0),
                 vec4(1.0,0.0,1.0,0.0)
                );
*/

void main(void)
{
	texCoord = gl_TexCoord[0].st / size0 + vec2(.0,.0);

	vec2 img    = texCoord * steps;
	vec4 pel    = texture2DRect( tex0, fract(img) * size0);

	vec4 tint;

	// Magenta Blue
	// Cyan    Yellow
	int ofs = int(texCoord.x*steps) + int(texCoord.y*steps) * 2;
	if (0 == ofs) {
		tint = vec4(1.0,1.0,0.0,0.0);
		pel = texture2DRect( tex0, vec2( (1.0-fract(img.x)) * size0.x, (1.0-fract(img.y)) * size0.y) );
	} else if (1 == ofs) {
		tint = vec4(0.0,0.0,1.0,0.0);
		pel = texture2DRect( tex0, vec2((fract(img.x)) * size0.x, (1.0-fract(img.y)) * size0.y) );
	} else if (2 == ofs) {
		tint = vec4(1.0,0.0,1.0,0.0);
		pel = texture2DRect( tex0, vec2((1.0-fract(img.x)) * size0.x, (fract(img.y)) * size0.y) );
	} else { // (3 == ofs)
		tint = vec4(0.0,1.0,1.0,0.0);
	};

	float gray  = dot(pel.rgb, vec3(0.3, 0.59, 0.11));

	gl_FragColor = mix( pel, tint, gray );
}