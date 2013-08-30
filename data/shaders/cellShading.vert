#version 120

varying vec3 normal, lightDir;
varying vec4 color;

void main()
{
        lightDir = normalize(vec3(gl_LightSource[0].position));
        normal = normalize(gl_NormalMatrix * gl_Normal);
        color = gl_Color;

        gl_Position = ftransform();
}