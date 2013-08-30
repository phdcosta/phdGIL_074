
uniform sampler2DRect offsetTexture;
uniform sampler2DRect sourceTexture;
uniform float height;
uniform float horizon;
uniform float power;

float map(float _value, float _minIn, float _maxIn, float _minOut, float _maxOut){
	return ((_value - _minIn) / (_maxIn - _minIn) * (_maxOut - _minOut) + _minOut);
}

void main(){
	vec2 st = gl_TexCoord[0].st;

	// float horizonLine = height; //horizon * height;
	float actualOffset = texture2DRect(offsetTexture, vec2(st.x,0.5)).r;
	float horizonOffset = actualOffset * height;
	float offset = 0.0;

	vec4 color = vec4(0.0,0.0,0.0,0.0);

	//	Height become the middle of the target
	//
	if (st.y < height){
		//	Top
		//
	    if (st.y < horizonOffset){
	    	//	Alpha Top
	    	//
	    	color = vec4(0.0,0.0,0.0,0.0);
	    } else {
	    	//	Profile Top
	    	//
	    	float pct = map(st.y, 0.0, height, 0.0, 1.0);

	    	pct = pow(pct, power);

	    	pct *= horizonOffset;
	    	//(st.y-horizonOffset)/(height-actualOffset);
	    	//offset = map(pct,0.0,1.0,0.0,horizonOffset);
	    	color = texture2DRect(sourceTexture, vec2(st.x, pct));
	    } 
	} else {
		//	Buttom
		//
		offset = st.y + horizonOffset - height;
		if ( st.y < (height*2.0-horizonOffset) ){
			//	Profile Buttom
			//
			color = texture2DRect(sourceTexture, vec2(st.x, offset));
		} else {
			//	Alpha Buttom
			//
			color = vec4(0.0,0.0,0.0,0.0);
		}
	}

	gl_FragColor = color;
}