#include "Rd.h"

//--------------------------------------------------------------
void Rd::setup(int _w, int _h){
	w = _w;
	h = _h;
	arrayLength = w * h;
	setupNeighbourMap();
	setupDefaults();
	image.allocate(w, h, OF_IMAGE_COLOR);
}

//--------------------------------------------------------------
// Create a neighbor map for neighbour lookup acceleration
void Rd::setupNeighbourMap(){
	for(int i = 0; i < w; i++){
		for(int j = 0; j < h; j++){
			vector <int> neighbours;
			int p = i + j * w;
			if(i == 0){
				neighbours.push_back(p + (w - 1));
			}
			else{
				neighbours.push_back(p - 1);
			}
			if(i == w - 1){
				neighbours.push_back(p - (w - 1));
			}
			else{
				neighbours.push_back(p + 1);
			}
			if(j == 0){
				neighbours.push_back(p + w * (h - 1));
			}
			else{
				neighbours.push_back(p - w);
			}
			if(j == h - 1){
				neighbours.push_back(p - w * (h - 1));
			}
			else{
				neighbours.push_back(p + w);
			}
			N.push_back(neighbours);
		}
	}
}

//--------------------------------------------------------------
// Setup arrays and default values
void Rd::setupDefaults(){
	A.resize(arrayLength);
	An.resize(arrayLength);
	Ad.resize(arrayLength);

	B.resize(arrayLength);
	Bn.resize(arrayLength);
	Bd.resize(arrayLength);

	D.resize(arrayLength);

	for(int i = 0; i < arrayLength; i++){
		A[i] = An[i] = 1.0f;
		B[i] = Bn[i] = 0.0f;

		Ad[i] = 0.5f;
		Bd[i] = 0.25f;
	}
}

//--------------------------------------------------------------
void Rd::setFeedRates(float f0, float f1){
	F[0] = f0;
	F[1] = f1;
}

//--------------------------------------------------------------
void Rd::setKillRates(float k0, float k1){
	K[0] = k0;
	K[1] = k1;
}

//--------------------------------------------------------------
void Rd::step(int numSteps){
	for(int i = 0; i < numSteps; i++){
		diffusion();
		reaction();
	}
}

//--------------------------------------------------------------
// Diffusion method (makes use of a neighbour map for speedup)
void Rd::diffusion(){
	for(int i = 0; i < w; i++){
		for(int j = 0; j < h; j++){
			int p = i + j * w;
			vector <int> P = N[p];
			An[p] = A[p] + Ad[p] * ((A[P[0]] + A[P[1]] + A[P[2]] + A[P[3]] - 4 * A[p]) / 4.0f);
			Bn[p] = B[p] + Bd[p] * ((B[P[0]] + B[P[1]] + B[P[2]] + B[P[3]] - 4 * B[p]) / 4.0f);
		}
	}
	// after calculating next matrix, copy it into current matrix
	A = An;
	B = Bn;
}

//--------------------------------------------------------------
// Reaction method (Gray-Scott)
void Rd::reaction(){
	for(int i = 0; i < w; i++){
		for(int j = 0; j < h; j++){
			int p = i + j * w;
			float a = A[p];
			float b = B[p];
			float ab2 = a * b * b;
			// use the division rate to determine this cells feed and kill rate
			float feedRate = D[p] * F[0] + (1 - D[p]) * F[1];
			float killRate = D[p] * K[0] + (1 - D[p]) * K[1];
			A[p] = A[p] - ab2 + feedRate * (1.0f - a);
			B[p] = B[p] + ab2 - (feedRate + killRate) * b;
		}
	}
}

//--------------------------------------------------------------
// randomly set substance values to kickstart the simulation
void Rd::kickstart(int num){
	for(int i = 0; i < num; i++){
		B[int(ofRandom(arrayLength))] = 1.0f;
	}
}

//--------------------------------------------------------------
// set the division rates based on the ALPHA (!) values of the input image
void Rd::setImage(ofPixels input){
	input.resize(w, h); // resize input image to simulation dimensions
	int numChannels = input.getNumChannels();
	for(int i = 0; i < arrayLength; i++){
		int a = input[i * numChannels + 3]; // get alpha of pixel
		D[i] = a / 255.0;
	}
}

//--------------------------------------------------------------
// return the visual output of the simulation
ofImage Rd::getImage(ofColor c1, ofColor c2){
	for(int y = 0; y < image.height; y++){
		for(int x = 0; x < image.width; x++){
			int index = x + y * w;
			ofColor c = c1;
			c.lerp(c2, A[index] * A[index]);
			image.setColor(x, y, c);
		}
	}
	image.update();
	return image;
}
