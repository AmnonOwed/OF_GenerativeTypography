#pragma once

#include "ofMain.h"
#include "Sphere.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		ofPath resamplePath(ofPath path, float spacing);
		ofMesh createMeshFromPath(ofPath path, float offSet);
		void getBoundaries(ofMesh & mesh);

		ofVec2f center;
		bool bResample;
		float resampleSpacing;
		float letterThickness;

		ofMesh mesh;
		ofVec3f boundsMin;
		ofVec3f boundsMax;

		int numSpheres;
		int addSpeed;
		vector <Sphere *> spheres;

		ofLight directionalLight;

		ofEasyCam cam;
};
