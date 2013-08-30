#pragma once

#include <ofMain.h>

/**
    \class  Warper
    \brief  a gl matrix transform quad warper
**/
class phdWarper {

	public:
		GLfloat	glWarpMatrix[16];

		phdWarper();
		virtual ~phdWarper() {}
		
		// transformation matrix
		void resetWarpMatrix();
		void calcWarpMatrix();
		
		/// set/get the warp points
		/// index: 0 - upper left, 1 - upper right, 2 - lower right, 3 - lower left 
		void setTarget(unsigned int index, double _x, double _y);
		void setSource(unsigned int index, double _x, double _y);

		void getSource(unsigned int index, double &_x, double &_y);
		void getTarget(unsigned int index, double &_x, double &_y);

		void setSrcQuad(double _x1, double _y1, double _x2, double _y2, double _x3, double _y3, double _x4, double _y4);
		void setDstQuad(double _x1, double _y1, double _x2, double _y2, double _x3, double _y3, double _x4, double _y4);
		
		void warpPoint(float srcX, float srcY, float& dstX, float& dstY);

		int findNearestSource(double _x, double _y);
		int findNearestTarget(double _x, double _y);

		void getBounds(double & _x, double & _y, double & _w, double & _h); 

		void setSize(double _w, double _h);
		void getSize(double & _w, double & _h);

		void setSides(int _sides);
		int getSides();

		void setParalelogram(bool _src, bool _dst) { srcParalelogram = _src; dstParalelogram = _dst; }
		bool getSrcParalelogram() { return srcParalelogram; }
		bool getDstParalelogram() { return dstParalelogram; }

		void setup(double _w, double _h, int _sides);
		void setup(double _srcW, double _srcH, double _dstW, double _dstH) {
			sides = 4;
			setSrcQuad(0, 0, _srcW, 0, _srcW, _srcH, 0, _srcH);
			setDstQuad(0, 0, _dstW, 0, _dstW, _dstH, 0, _dstH);
		}

		void setEdgeBased(bool _value) { edgeBased = _value; }
		bool getEdgeBased() { return edgeBased; }
		void setEdgeSizes(double _a, double _b, double _c, double _angle) { sizeA = _a; sizeB = _b; sizeC = _c; angle = _angle; }

		void setAsString(string & _setup);
		string getAsString();

		void begin();
		void end();

	private:

		bool edgeBased;
		double sizeA, sizeB, sizeC, angle;

		double width;
		double height;

		int sides; // 3 or 4
		bool srcParalelogram;
		bool dstParalelogram;

		// projection warp points
		double	_dst[4][2];
		double	_src[4][2];
		
		// projection warping matrices
		double	_warpMatrix[3][3];

		// control if updated
		bool changed;
};

class phdWarpGrid {
public:
private:
};
