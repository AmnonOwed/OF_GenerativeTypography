#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		ofPath resamplePath(ofPath path, float spacing);
		ofMesh createMeshFromPath(ofPath path, float offSet);
		void colorFaces(ofMesh & mesh);

		ofVec2f center;
		ofMesh mesh;

		bool bResample;
		float resampleSpacing;
		float letterThickness;

		ofEasyCam cam;
};
