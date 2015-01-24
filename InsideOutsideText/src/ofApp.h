#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void mousePressed(int x, int y, int button);

		int lineLength;
		bool reverseDrawing;
		ofColor ellipse_color;
		ofColor line_color;
		ofColor fbo_color;

		ofPixels pix;
};
