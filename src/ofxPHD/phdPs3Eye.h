//--------------------------------------------------------------
//--------------------------------------------------------------
// PS3EYE
//--------------------------------------------------------------
#include "ofMain.h"
#include "ps3eyeLib\IPS3EyeLib.h"

#define PS3_USE_OPENCV

#ifdef PS3_USE_OPENCV
#include "ofxOpenCv.h"
#endif

class phdPs3Eye {
public:
	IPS3EyeLib		*ps3eyeCam;
	int				ps3eyeWidth;
	int				ps3eyeHeight;
	int				ps3eyeFPS;
	PBYTE			ps3eyeBuffer;
	bool			ps3eyeBGR2RGB;
	bool			ps3eyeNewFrame;
	ofImage			ps3eyeColorImage;

#ifdef PS3_USE_OPENCV
	ofxCvColorImage	ps3eyeCvColor;
#endif

	phdPs3Eye() {
	}

	bool setup(int _width, int _height, int _fps, bool _bgr2rgb) {

		int nCam = IPS3EyeLib::GetNumCameras();

		if(nCam < 1) return false;

		ps3eyeWidth = _width;
		ps3eyeHeight = _height;
		ps3eyeFPS = _fps;
		ps3eyeBGR2RGB = _bgr2rgb;

		// cria a camera PS3Eye
		ps3eyeCam = IPS3EyeLib::Create();
		ps3eyeCam->SetFormat(IPS3EyeLib::GetFormatIndex(_width,_height,_fps));
		
		// aloca os buffers de imagens
		ps3eyeBuffer = new BYTE[_width*_height*3]; // 3 bytes per pixel
		
		// comeca a capturar imagens
		ps3eyeCam->StartCapture();

		// aloca as imagens que serão usadas
		ps3eyeColorImage.allocate(_width, _height, OF_IMAGE_COLOR);
		ps3eyeColorImage.setUseTexture(true);

#ifdef PS3_USE_OPENCV
		ps3eyeCvColor.allocate(_width,_height);
		ps3eyeCvColor.setUseTexture(false);
#endif

		return true;
	}

	void update() {

		ps3eyeNewFrame = false;
		ps3eyeNewFrame = ps3eyeCam->GetFrame(ps3eyeBuffer, 24, false, true);

		if(ps3eyeNewFrame) {

#ifdef PS3_USE_OPENCV
			ps3eyeCvColor.setFromPixels(ps3eyeBuffer, ps3eyeWidth, ps3eyeHeight);
			if(ps3eyeBGR2RGB) cvCvtColor(ps3eyeCvColor.getCvImage(), ps3eyeCvColor.getCvImage(), CV_BGR2RGB);
			ps3eyeCvColor.mirror(false, true);
			ps3eyeColorImage.setFromPixels(ps3eyeCvColor.getPixels(), ps3eyeWidth, ps3eyeHeight, OF_IMAGE_COLOR);
#else
			if(ps3eyeBGR2RGB) {
				unsigned char * p = ps3eyeBuffer;
				for(int i = 0; i < ps3eyeWidth*ps3eyeHeight*3; i+= 3) { unsigned char a = p[i+2]; p[i+2] = p[i]; p[i] = a; }
			}
			ps3eyeColorImage.setFromPixels(ps3eyeBuffer, ps3eyeWidth, ps3eyeHeight, OF_IMAGE_COLOR);
			ps3eyeColorImage.mirror(false, true);
#endif
		}
	}

	void draw() {
		draw(0,0,ps3eyeWidth,ps3eyeHeight);
	}

	void draw(float _x, float _y) {
		draw(_x,_y,ps3eyeWidth,ps3eyeHeight);
	}

	void draw(float _x, float _y, float _w, float _h) {
		ps3eyeColorImage.draw(_x,_y,_w,_h);
	}
};
