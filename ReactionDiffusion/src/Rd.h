#ifndef RD_H
#define RD_H

#include "ofMain.h"

class Rd {
	public:
		void setup(int w, int h);
		void setupNeighbourMap();
		void setupDefaults();
		void setFeedRates(float f0, float f1);
		void setKillRates(float k0, float k1);
		void step(int numSteps);
		void diffusion();
		void reaction();
		void kickstart(int num);
		void setImage(ofPixels input);
		void getImage(ofImage & image, const ofColor & c1, const ofColor & c2);

		int w, h; // 2D dimensions
		int vectorSize; // Size for all the vectors (w * h)
		vector <float> A, An, Ad; // Substance A (value, next value, diffusion rate)
		vector <float> B, Bn, Bd; // Substance B (value, next value, diffusion rate)
		vector <vector <int> > N; // Neighbour references for diffusion method (4 in 2D: left, right, up, down)
		float F[2]; // Feed rates (2)
		float K[2]; // Kill rates (2)
		vector <float> D; // Division rates
};

#endif // RD_H
