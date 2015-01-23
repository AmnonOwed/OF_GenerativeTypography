#pragma once

#include "ofMain.h"

#include "Particle.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void mousePressed(int x, int y, int button);

		int maxParticles;
		int drawMode;

		ofColor BACKGROUND_COLOR;
		ofColor FBO_COLOR;

        ofFbo fbo;
		ofPixels pix;

		vector<Particle> particles;
};
