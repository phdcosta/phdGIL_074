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

vec2 zoom0 = vec2(2., 2.);

float pi = 3.1415927410125;

float stitching_size = 6.0;
int invert = 1;

vec4 PostFX(sampler2DRect tex, vec2 uv, float time){
  vec4 c = vec4(0.0);
  float size = stitching_size;
  vec2 cPos = uv * size0;
  vec2 tlPos = floor(cPos / vec2(size, size));
  tlPos *= size;
  int remX = int(mod(cPos.x, size));
  int remY = int(mod(cPos.y, size));
  if (remX == 0 && remY == 0) tlPos = cPos;
  vec2 blPos = tlPos;
  blPos.y += (size - 1.0);
  if ((remX == remY) || (((int(cPos.x) - int(blPos.x)) == (int(blPos.y) - int(cPos.y))))){
    if (invert == 1)
      c = vec4(0.2, 0.15, 0.05, 1.0);
    else
      c = texture2DRect(tex, tlPos) * 1.4;
  } else {
    if (invert == 1)
      c = texture2DRect(tex, tlPos) * 1.4;
    else
      c = vec4(0.0, 0.0, 0.0, 1.0);
  }
  return c;
}

void main( void ) {
  vec2 uv = gl_TexCoord[0].st / size0 + vec2(0.0, 0.0);
  gl_FragColor = PostFX(tex0, uv, time);
}

