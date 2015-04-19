#pragma once

#include "ofMain.h"

#include "ofxPointInMesh.h"
#include "ofxVoro.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		ofPath resamplePath(ofPath path, float spacing);
		ofMesh createMeshFromPath(ofPath path, float offSet);
		void getBoundaries(ofMesh & mesh);

		void generatePoints(int numPoints);
		ofVec3f getPointInMesh();
		ofVec3f getRandomPointWithinBounds();
		void createVoronoi(float wallThickness);
		void generateColors(float start);
		void calculateCenter();

		void keyPressed(int key);

		bool bResample;
		float resampleSpacing;
		float letterThickness;

		ofMesh mesh;
		ofVec3f boundsMin;
		ofVec3f boundsMax;
		ofVec3f center;

		bool bReset;

		vector <ofPoint> points;
		vector <ofVboMesh> cellMeshes;
		vector <ofVboMesh> cellMeshWireframes;
		vector <ofColor> colors;

		ofLight directionalLight;
		ofEasyCam cam;
};
