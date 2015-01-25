#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void updateDrawMode();

		void mousePressed(int x, int y, int button);

		int drawSpeed;
		int drawMode;

		bool bUpdateDrawMode;

		ofColor bg_color;
		ofColor fbo_color;

		ofPixels pix;
};
