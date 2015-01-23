#ifndef PARTICLE_H
#define PARTICLE_H

#include "ofMain.h"

class Particle {
	public:
		Particle(ofPixels & _pix, ofColor & _c);

		void getPosition();
		void update(const int & drawMode);
		void display(const int & drawMode);
		bool isInText();

		ofPixels pix;
		ofColor c;

		ofVec2f loc;
		ofVec2f vel;

		float radius;
		float life;
		float lifeRate;
};

#endif // PARTICLE_H
