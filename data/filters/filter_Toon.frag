#version 150

//#extension GL_ARB_texture_rectangle : enable

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

#define HueLevCount 6
#define SatLevCount 7
#define ValLevCount 4

float[HueLevCount] HueLevels = float[] (0.0,80.0,160.0,240.0,320.0,360.0);
float[SatLevCount] SatLevels = float[] (0.0,0.15,0.3,0.45,0.6,0.8,1.0);
float[ValLevCount] ValLevels = float[] (0.0,0.3,0.6,1.0);

vec3 RGBxxHSV( float r, float g, float b) {
   float minv, maxv, delta;
   vec3 res;

   minv = min(min(r, g), b);
   maxv = max(max(r, g), b);
   res.z = maxv;            // v
   
   delta = maxv - minv;

   if( maxv != 0.0 )
      res.y = delta / maxv;      // s
   else {
      // r = g = b = 0      // s = 0, v is undefined
      res.y = 0.0;
      res.x = -1.0;
      return res;
   }

   if( r == maxv )
      res.x = ( g - b ) / delta;      // between yellow & magenta
   else if( g == maxv )
      res.x = 2.0 + ( b - r ) / delta;   // between cyan & yellow
   else
      res.x = 4.0 + ( r - g ) / delta;   // between magenta & cyan

   res.x = res.x * 60.0;            // degrees
   if( res.x < 0.0 )
      res.x = res.x + 360.0;
      
   return res;
}

vec3 HSVtoRGB(float m,float n,float o) {
    vec3 hsv = vec3(m,n,o);
    vec3 col;
    float hue = mod(hsv.r, 360.0);
    float s = max(0, min(1, hsv.g));
    float v = max(0, min(1, hsv.b));
    if(s > 0.0) {
        int h = int(floor(hue / 60.0));
        float f = hue / 60.0 - float(h);
        float p = v * (1.0 - s);
        float q = v * (1.0 - f * s);
        float r = v * (1.0 - (1.0 - f) * s);
 
        if(h == 0) col = vec3(v, r, p);
        else if(h == 1) col = vec3(q, v, p);
        else if(h == 2) col = vec3(p, v, r);
        else if(h == 3) col = vec3(p, q, v);
        else if(h == 4) col = vec3(r, p, v);
        else col = vec3(v, p, q);
    }else{
        col = vec3(v);
    }
    return col;
}

vec3 HSVkkRGB(float h,float s,float v) {
	return mix(vec3(1.),clamp((abs(fract(h+vec3(3.,2.,1.)/3.)*6.-3.)-1.),0.,1.),s)*v;
}

vec3 HSVyyRGB(float h, float s, float v ) {
   int i;
   float f, p, q, t;
   vec3 res;

   if( s == 0.0 ) {
      // achromatic (grey)
      res.x = v;
      res.y = v;
      res.z = v;
      return res;
   }

   h /= 60.0;         // sector 0 to 5
   i = int(floor( h ));
   f = h - float(i);         // factorial part of h
   p = v * ( 1.0 - s );
   q = v * ( 1.0 - s * f );
   t = v * ( 1.0 - s * ( 1.0 - f ) );

   switch( i ) {
      case 0:
         res.x = v;
         res.y = t;
         res.z = p;
         break;
      case 1:
         res.x = q;
         res.y = v;
         res.z = p;
         break;
      case 2:
         res.x = p;
         res.y = v;
         res.z = t;
         break;
      case 3:
         res.x = p;
         res.y = q;
         res.z = v;
         break;
      case 4:
         res.x = t;
         res.y = p;
         res.z = v;
         break;
      default:      // case 5:
         res.x = v;
         res.y = p;
         res.z = q;
         break;
   }
   
   return res;
}

vec3 RGByyHSV(float r, float g, float b)
{
    float delta; //r, g, b, 
    float colorMax, colorMin;
    float h = 0.0, s = 0.0, v = 0.0;
    vec4 hsv = vec4(0.0);
    //r = color[0];    g = color[1];     b = color[2];
    colorMax = max(r,g);    colorMax = max(colorMax,b);
    colorMin = min(r,g);    colorMin = min(colorMin,b);
    v = colorMax;                // this is value
    
    if(colorMax != 0.0)
    {
      s = (colorMax - colorMin ) / colorMax;
    }
    
    if(s != 0.0)
    {
        delta = colorMax - colorMin;
        if(r == colorMax)
        {
            h = (g - b) / delta;
        }
        else if(g == colorMax)
        {        
            h = 2.0 + (b - r) / delta;
        }
        else // b is max
        {    
            h = 4.0 + (r - g) / delta;
        }
        
        h *= 60.0;
        
        if(h < 0.0)
        {
            h += 360.0;
        }
        
        // this is flawed if the target is 8 bit!!!!!
        hsv[0] = h / 360.0;     // moving h to be between 0 and 1.
        hsv[1] = s;
    }

    hsv[2] = v;
    return hsv.xyz;
}


vec3 HSVxxRGB(float h, float s, float v)
{
    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
    float f,p,q,t;//, h,s,v;
    float r = 0.0, g = 0.0, b = 0.0, i;
    
    if(s == 0.0)
    {
        if(v != 0.0)
        {
            color = vec4(v);        // black and white case
        }
    }
    else
    {        // this next step is flawed if the texels are 8 bit!!
        h *= 360.; //h = hsv.x * 360.0;
        //s = hsv.y;
        //v = hsv.z;
        if(h == 360.0)
        {   
            h = 0.0;
        }

        h /= 60.0;
        i = floor(h);
        f = h-i;
        p = v * (1.0 - s);
        q = v * (1.0 -(s * f));
        t = v * (1.0 -(s * (1.0 -f)));
        if( i == 0)        {             r = v;            g  = t; b = p; }
        else if( i== 1 )   {             r = q;            g  = v; b = p; }
        else if( i== 2 )   {             r = p;            g  = v; b = t; }
        else if( i== 3 )   {             r = p;            g  = q; b = v; }
        else if( i== 4 )   {             r = t;            g  = p; b = v; }
        else if( i== 5 )   {             r = v;            g  = p; b = q; }
        color.r = r;        color.g = g;         color.b = b;
    }
    return color.rgb;
}

vec3 RGBtoHSV(float r, float g, float b) {
	// individual variables for RGB channels
	float var_R = r;
	float var_G = g;
	float var_B = b;

	float var_Min = min( min( var_R, var_G ), var_B );	//Min. value of RGB
	float var_Max = max( max( var_R, var_G ), var_B );	//Max. value of RGB
	float del_Max = var_Max - var_Min;			//Delta RGB value

	float V = var_Max;
	float H;
	float S;

	if ( del_Max == 0. )			//This is a gray, no chroma...
	{
		H = 0.;				//HSL results from 0 to 1
		S = 0.;
	}
	else					//Chromatic data...
	{
		S = del_Max / var_Max;

		float del_R = ( ( ( var_Max - var_R ) / 6. ) + ( del_Max / 2. ) ) / del_Max;
		float del_G = ( ( ( var_Max - var_G ) / 6. ) + ( del_Max / 2. ) ) / del_Max;
		float del_B = ( ( ( var_Max - var_B ) / 6. ) + ( del_Max / 2. ) ) / del_Max;

		if( var_R == var_Max ) 
			H = del_B - del_G;
		else if( var_G == var_Max )
			H = ( 1. / 3. ) + del_R - del_B;
		else if( var_B == var_Max ) 
			H = ( 2. / 3. ) + del_G - del_R;

		if ( H < 0. ) H = H + 1.;
		if ( H > 1. ) H = H - 1.;
	}
	return vec3(var_R, var_G, var_B);
}

vec3 HSVzzRGB(float h,float s,float v) {

	// individual variables for HSL channels
	float H = h;
	float S = s;
	float V = v;

	float R;
	float G;
	float B;

	if ( S == 0. )				//HSL from 0 to 1
	{
		R = V;
		G = V;
		B = V;
	}
	else
	{
		float var_h = H * 6.;
		if( var_h == 6. ) var_h = 0.;

		float var_i = floor( var_h );
		float var_1 = V * ( 1. - S );
		float var_2 = V * ( 1. - S * ( var_h - var_i ) );
		float var_3 = V * ( 1. - S * ( 1. - ( var_h - var_i ) ) );

		if	( var_i == 0. )	{ R = V     ; 	G = var_3 ; 	B = var_1	; }
		else if	( var_i == 1. )	{ R = var_2 ; 	G = V     ; 	B = var_1	; }
		else if	( var_i == 2. )	{ R = var_1 ; 	G = V     ; 	B = var_3	; }
		else if	( var_i == 3. )	{ R = var_1 ; 	G = var_2 ; 	B = V		; }
		else if	( var_i == 4. )	{ R = var_3 ; 	G = var_1 ; 	B = V		; }
		else			{ R = V     ; 	G = var_1 ; 	B = var_2	; }

	}

	// output
	return vec3( R, G, B);
}

float nearestLevel(float col, int mode) {
   int levCount;
   if (mode==0) levCount = HueLevCount;
   if (mode==1) levCount = SatLevCount;
   if (mode==2) levCount = ValLevCount;
   
   for (int i =0; i<levCount-1; i++ ) {
     if (mode==0) {
        if (col >= HueLevels[i] && col <= HueLevels[i+1]) {
          return HueLevels[i+1];
        }
     }
     if (mode==1) {
        if (col >= SatLevels[i] && col <= SatLevels[i+1]) {
          return SatLevels[i+1];
        }
     }
     if (mode==2) {
        if (col >= ValLevels[i] && col <= ValLevels[i+1]) {
          return ValLevels[i+1];
        }
     }
   }
}

// averaged pixel intensity from 3 color channels
float avg_intensity(vec4 pix) {
 return (pix.r + pix.g + pix.b)/3.;
}

vec4 get_pixel(vec2 coords, float dx, float dy) {
 return texture2DRect(tex0, coords + vec2(dx, dy));
}

// returns pixel color
float IsEdge(in vec2 coords){
  float dxtex = 1.0;// / size0.x;//float(textureSize(tex0,0)) ;
  float dytex = 1.0;// / size0.y;//float(textureSize(tex0,0));
  float pix[9];
  int k = -1;
  float delta;

  // read neighboring pixel intensities
  for (int i=-1; i<2; i++) {
   for(int j=-1; j<2; j++) {
    k++;
    pix[k] = avg_intensity(get_pixel(coords, float(i)*dxtex, float(j)*dytex));
   }
  }

  // average color differences around neighboring pixels
  delta = (abs(pix[1]-pix[7])+ abs(pix[5]-pix[3]) +
          abs(pix[0]-pix[8]) + abs(pix[2]-pix[6])
           )/4.;

  return clamp(5.5*delta,0.0,1.0);
}

void main(void){

    vec2 p = gl_TexCoord[0].st / size0 + vec2(.0,.0);

    vec4 colorOrg = texture2DRect(tex0, p*size0);

    vec3 vHSV =  RGBtoHSV(colorOrg.r,colorOrg.g,colorOrg.b);

    vHSV.x = nearestLevel(vHSV.x, 0);
    vHSV.y = nearestLevel(vHSV.y, 1);
    vHSV.z = nearestLevel(vHSV.z, 2);

    float edg = IsEdge(p*size0);

    vec3 vRGB = (edg >= 0.3)? vec3(0.0,0.0,0.0):HSVtoRGB(vHSV.x,vHSV.y,vHSV.z);

    gl_FragColor = vec4(vRGB.x,vRGB.y,vRGB.z,1.0);
}