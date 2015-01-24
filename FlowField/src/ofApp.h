#pragma once

#include "ofMain.h"

#include "Particle.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void resetParticles();

		void mousePressed(int x, int y, int button);

		int maxParticles;
		int drawMode;

		ofColor bg_color;
		ofColor fbo_color;

		ofFbo fbo;
		ofPixels pix;

		vector <Particle> particles;
		bool bReset;
};
