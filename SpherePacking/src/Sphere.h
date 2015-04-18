#ifndef SPHERE_H
#define SPHERE_H

#include "ofMain.h"
#include "ofxPointInMesh.h"

class Sphere : public ofSpherePrimitive {

	public:
		void setPointers(ofMesh * _meshPtr, ofVec3f * _boundsMinPtr, ofVec3f * _boundsMaxPtr, vector <Sphere *> * _spheresPtr);

		void setup();
		void update();
		void draw();

	private:
		ofVec3f getPointInMesh();
		ofVec3f getRandomPointWithinBounds();
		void checkOverlap();

		ofMesh * meshPtr;
		ofVec3f * boundsMinPtr;
		ofVec3f * boundsMaxPtr;
		vector <Sphere *> * spheresPtr;

		float growRate;
		bool isOverlap;
		ofColor c;
};

#endif // SPHERE_H
