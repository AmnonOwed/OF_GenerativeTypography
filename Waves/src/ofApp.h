#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		int gridX;
		int gridY;
		float waveHeight;
		float baseHeight;

		ofPath path;
		ofColor stroke_color;
		ofColor fill_color;

		ofColor bg_color;
		ofColor fbo_color;

		ofPixels pix;
};
