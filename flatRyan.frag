


uniform sampler2DRect offsetTexture;
uniform sampler2DRect sourceTexture;
uniform float height;
uniform float horizon;
uniform float transitionSmoothing;

float map(float _value, float _minIn, float _maxIn, float _minOut, float _maxOut){
	return ((_value - _minIn) / (_maxIn - _minIn) * (_maxOut - _minOut) + _minOut);
}

void main(){
	vec2 st = gl_TexCoord[0].st;

	float horizonLine = horizon * height;
	float actualOffset = texture2DRect(offsetTexture, vec2(st.x,0.5)).r;
	float horizonOffset = actualOffset * height;
	float offset = st.y + horizonOffset - horizonLine;

	vec4 color = vec4(0.0,0.0,0.0,0.0);

	if (st.y < horizonLine){
	    float pct = 1.0 - (st.y/horizonOffset);
	    offset = map(pct,1.0,0.0,0.0,horizonOffset);
	    if (offset < horizonOffset){
	    	color = texture2DRect(sourceTexture, vec2(st.x, offset));
	    } else {
	    	color = vec4(0.0,0.0,0.0,0.0);
	    }
	    
	} else {
		color = texture2DRect(sourceTexture, vec2(st.x, offset));

	}

	if ( (st.y > (height-horizonOffset+horizonLine))  || (offset < 0.0 )){
		color = vec4(0.0,0.0,0.0,0.0);
	}

	gl_FragColor = color;
}