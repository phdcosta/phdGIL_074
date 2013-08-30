varying vec2 coords;

void main() {
	coords = gl_Vertex.xy;
	gl_Position = vec4(coords, 1.0, 1.0);
}