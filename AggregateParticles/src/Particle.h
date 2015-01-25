#ifndef PARTICLE_H
#define PARTICLE_H

#include "ofMain.h"

class Particle {
	public:
		Particle();

		void setup(ofPixels * _pix, ofColor _c, int _drawMode);
		void update();
		void display();
		void getPosition();
		bool isInText();

		ofPixels * pixPtr;
		ofColor c;
		int drawMode;

		ofVec2f loc;
		ofVec2f vel;

		float radius;
		float life;
		float lifeRate;
};

#endif // PARTICLE_H
