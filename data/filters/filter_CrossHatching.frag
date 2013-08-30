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

float hatch_y_offset = 5.0;
float lum_threshold_1 = 1.0;
float lum_threshold_2 = 0.7;
float lum_threshold_3 = 0.5;
float lum_threshold_4 = 0.3;

void main( void ) {

    vec2 uv = gl_TexCoord[0].st / size0 + vec2(0.0, 0.0);
    vec2 pos = gl_FragCoord.xy;

    float lum = length(texture2DRect(tex0, uv * size0).rgb);

    vec3 tc = vec3(1.0, 1.0, 1.0);
  
    if (lum < lum_threshold_1) {
      if (mod(pos.x + pos.y, 10.0) == 0.0) tc = vec3(0.0, 0.0, 0.0);
    }  
  
    if (lum < lum_threshold_2) {
      if (mod(pos.x - pos.y, 10.0) == 0.0) tc = vec3(0.0, 0.0, 0.0);
    }  
  
    if (lum < lum_threshold_3) {
      if (mod(pos.x + pos.y - hatch_y_offset, 10.0) == 0.0) tc = vec3(0.0, 0.0, 0.0);
    }  
  
    if (lum < lum_threshold_4) {
      if (mod(pos.x - pos.y - hatch_y_offset, 10.0) == 0.0) tc = vec3(0.0, 0.0, 0.0);
    }

    gl_FragColor = vec4(tc, 1.0);
}

