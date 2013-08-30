#version 150

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

void main(void){

	vec2 bgl_TextureCoordinateOffset[9];

	bgl_TextureCoordinateOffset[0] = vec2(-1,-1);
	bgl_TextureCoordinateOffset[1] = vec2( 0,-1);
	bgl_TextureCoordinateOffset[2] = vec2( 1,-1);

	bgl_TextureCoordinateOffset[3] = vec2(-1, 0);
	bgl_TextureCoordinateOffset[4] = vec2( 0, 0);
	bgl_TextureCoordinateOffset[5] = vec2( 1, 0);

	bgl_TextureCoordinateOffset[6] = vec2(-1, 1);
	bgl_TextureCoordinateOffset[7] = vec2( 0, 1);
	bgl_TextureCoordinateOffset[8] = vec2( 1, 1);

	vec2 p = gl_TexCoord[0].st / size0 + vec2(.0,.0);

	vec4 colorOrg = texture2DRect(tex0, p*size0);

	vec4 sample[9];
	vec4 border;
	vec4 texcol = texture2DRect(tex0, gl_TexCoord[0].st);

	for (int i = 0; i < 9; i++) {
		sample[i] = texture2DRect(tex0, gl_TexCoord[0].st + bgl_TextureCoordinateOffset[i]);
	}

	vec4 horizEdge = sample[2] + sample[5] + sample[8] - (sample[0] + sample[3] + sample[6]);

	vec4 vertEdge = sample[0] + sample[1] + sample[2] - (sample[6] + sample[7] + sample[8]);

	border.rgb = sqrt((horizEdge.rgb * horizEdge.rgb) + (vertEdge.rgb * vertEdge.rgb));

	if (border.r > 0.4||border.g > 0.4||border.b > 0.4){
		gl_FragColor.r = 0.0;
		gl_FragColor.g = 0.0;
		gl_FragColor.b = 0.0;
		gl_FragColor.a = 1.0;
	}else{
		gl_FragColor.rgb = texcol.rgb;
		gl_FragColor.a = 1.0;
	}
}