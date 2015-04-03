#pragma once

#include "ofMain.h"
#include "ofxVoronoi.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		vector <ofPoint> generateRandomPoints(int count);
		bool isPointInList(const ofPoint & point, const vector <ofPoint> & pointList);

		void keyPressed(int key);

		vector <ofPoint> textPoints;
		vector <bool> insideText;
		ofxVoronoi voronoi;

		int currentPoint;
		float interpolationBetweenPoints;

		bool displayPoints;
		bool saveOneFrame;
		bool reset;

		ofColor fbo_color;
		ofPixels pix;
};
