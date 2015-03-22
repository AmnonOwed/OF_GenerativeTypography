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
		ofVboMesh mesh; // the only difference from Basic3DType is the use of ofVboMesh, that's it! ;-)

		bool bResample;
		float resampleSpacing;
		float letterThickness;

		ofEasyCam cam;
};
