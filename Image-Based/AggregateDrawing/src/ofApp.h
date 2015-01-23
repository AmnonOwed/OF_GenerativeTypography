#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void mousePressed(int x, int y, int button);

		int drawSpeed;
		int drawMode;

		ofColor BACKGROUND_COLOR;
		ofColor FBO_COLOR;

        ofPixels pix;
};
