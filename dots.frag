
//Declare a 2D texture as a uniform variable
uniform sampler2DRect texture;
uniform float pixScaleX;
uniform float pixScaleY;

uniform float sharpness;

uniform float width;
uniform float height;

uniform float brightnessScale;
uniform float maxScale;
uniform float brightness;

float smootherstep(float x)
{
    // Scale, and saturate x to 0..1 range
    // Evaluate polynomial
    return x*x*x*(x*(x*6.0 - 15.0) + 10.0);
}


vec2 findClosestDot(vec2 coords, float xFactor, float yFactor) {
    
    
	// scale down the image
	coords.x *= xFactor;
	coords.y *= yFactor;
	
	float ymod = coords.y;
	
	coords.x = floor(coords.x+0.5);
	coords.y = floor(coords.y+0.5);
	int x = int(coords.x);
	ymod -= coords.y;
	float yOffset = 0.5 * sign(ymod);
	
	coords.y += yOffset*float(x - 2*(x/2));
    
    
	// scale up the image
	coords.x /= xFactor;
	coords.y /= yFactor;
	
	return coords;
}


float marekstep(float inp) {
    
	return 1.0 - (1.0/(1.0+exp(sharpness*(inp-0.5))));
}
float briScale(float brightness) {
    
    return  (1.0-brightnessScale) + (brightnessScale)*(1.0/brightness);
}
void main()
{
	vec2 coords = gl_TexCoord[0].xy;
	
	vec2 dims = vec2(width, height);
	
	float xFactor = pixScaleX;
	float yFactor = pixScaleY;
	
	vec4 color = texture2DRect(texture,coords*dims);
	
	vec2 quantCoords = findClosestDot(coords, xFactor, yFactor);
	
	vec4 quantColor = texture2DRect(texture,quantCoords*dims);
	
	float b = (quantColor.r+quantColor.g+quantColor.b)/3.0;
	
	vec2 v = (coords - quantCoords)*vec2(xFactor, yFactor)*briScale(b);
	float bri = 1.0 - length(v);
	bri = marekstep(bri);
	//bri = 1.0;
	/*bri *= (1.0 - b)*3.0;
     bri = 1.0 - bri;
     bri = 0.5 + bri*0.5;
     bri= smootherstep(smootherstep(bri));
     bri = 0.2;*/
	vec4 c = vec4(min(1.0, bri*quantColor.r*brightness), min(1.0,bri*quantColor.g*brightness), min(1.0, bri*quantColor.b*brightness), 1.0);
	//Multiply color by texture
	gl_FragColor = gl_Color * c;
}
