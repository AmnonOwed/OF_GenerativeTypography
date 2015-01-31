#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	saveOneFrame = false; // variable used to save a single frame as a PDF page
	bResetPointsAndColors = true; // variable used to trigger resetting the points and colors

	numPoints = 150; // the number of points generated
	maxDistance = 55; // maximum distance between two points to draw a line

	line_color = ofColor(0, 125); // color of interconnected lines
	bg_color = ofColor(255);
	fbo_color = ofColor(0);

	ofBackground(bg_color);
	ofEnableAntiAliasing();
	ofEnableSmoothing();
	ofSetCircleResolution(64); // increase the resolution for better quality circles
	ofSetFrameRate(60);

	ofTrueTypeFont ttf;
	ttf.loadFont(OF_TTF_SANS, 350);
	string s = "TYPE";

	ofFbo fbo;
	fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	pix.allocate(ofGetWidth(), ofGetHeight(), OF_PIXELS_RGBA);
	fbo.begin();
	ofClear(0, 0, 0, 0);
	// Center string code from:
	// https://github.com/armadillu/ofxCenteredTrueTypeFont/blob/master/src/ofxCenteredTrueTypeFont.h
	ofRectangle r = ttf.getStringBoundingBox(s, 0, 0);
	ofVec2f offset = ofVec2f(floor(-r.x - r.width * 0.5f), floor(-r.y - r.height * 0.5f));
	ofSetColor(fbo_color);
	ttf.drawString(s, fbo.getWidth() / 2 + offset.x, fbo.getHeight() / 2 + offset.y);
	fbo.end();

	fbo.readToPixels(pix); // the ofPixels class has a convenient getColor() method
}

//--------------------------------------------------------------
void ofApp::update(){
	if(bResetPointsAndColors){
		resetPointsAndColors();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	if(saveOneFrame){
		ofBeginSaveScreenAsPDF("LinesCircles-" + ofGetTimestampString() + ".pdf", false);
	}

	// draw colored ellipses
	ofFill();
	for(int i = 0; i < points.size(); i++){
		ofSetColor(colors[i]);
		ofVec3f p = points[i];
		ofCircle(p.x, p.y, p.z);
	}

	// draw lines between the points within a certain distance of each other
	ofNoFill();
	ofSetColor(line_color);
	ofSetLineWidth(0.5);
	for(int i = 0; i < points.size(); i++){
		ofVec3f p = points[i];
		for(int j = i + 1; j < points.size(); j++){
			ofVec3f q = points[j];
			if(ofDist(p.x, p.y, q.x, q.y) < maxDistance){
				ofLine(p.x, p.y, q.x, q.y);
			}
		}
	}

	if(saveOneFrame){
		ofEndSaveScreenAsPDF();
		saveOneFrame = false;
	}
}

//--------------------------------------------------------------
// generate the points based on the text shape, for each point generate a color
void ofApp::resetPointsAndColors(){
	points.clear();
	colors.clear();
	while(points.size() < numPoints){
		float x = ofRandom(ofGetWidth());
		float y = ofRandom(ofGetHeight());
		ofColor c = pix.getColor((int)x, (int)y);
		bool inText = (c == fbo_color);
		if(inText){
			float radius = ofRandom(1) < 0.075 ? ofRandom(10, 37.5) : ofRandom(1.5, 16.6);
			ofVec3f v = ofVec3f(x, y, radius);
			points.push_back(v);
			ofColor c;
			c.setHsb(ofRandom(255), 153, 216.75, 165.75);
			colors.push_back(c);
		}
	}
	bResetPointsAndColors = false;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == 's'){
		saveOneFrame = true; // set the variable to true to save a single frame as a PDF file / page
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	bResetPointsAndColors = true;
}
