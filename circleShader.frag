uniform sampler2D texture;
uniform vec2 center;
varying vec2 coords;

void main(){
	vec4 valueRed = texture2D(texture, coords);
	float distance = 1-length(center - coords);
	distance = sin(distance * 3.1416);
	valueRed.r += distance;
	gl_FragColor = valueRed;
}