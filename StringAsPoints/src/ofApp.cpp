#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	saveOneFrame = false; // variable used to save a single frame as a PDF page

	ofBackground(255);
	ofEnableAntiAliasing();
	ofEnableSmoothing();
	ofSetFrameRate(60);

	ofTrueTypeFont ttf;
	// to extract points, loadFont must be called with makeContours set to true
	ttf.loadFont(OF_TTF_SANS, 350, true, false, true);
	string s = "TYPE";

	// Center string code from:
	// https://github.com/armadillu/ofxCenteredTrueTypeFont/blob/master/src/ofxCenteredTrueTypeFont.h
	ofRectangle r = ttf.getStringBoundingBox(s, 0, 0);
	center = ofVec2f(floor(-r.x - r.width * 0.5f), floor(-r.y - r.height * 0.5f));
	center.x += ofGetWidth() / 2;
	center.y += ofGetHeight() / 2;

	// get the string as paths
	vector <ofTTFCharacter> paths = ttf.getStringAsPoints(s);
	for(int i = 0; i < paths.size(); i++){
		// for every character break it out to polylines
		vector <ofPolyline> polylines = paths[i].getOutline();
		// for every polyline...
		for(int j = 0; j < polylines.size(); j++){
			// vector to store all the points of a single polyline
			vector <ofVec2f> linePoints;
			// resample polyline to enforce equal spacing between points
			ofPolyline spacePoly = polylines[j].getResampledBySpacing(10);
			// add all the points of the resampled polyline to the character vector
			for(int k = 0; k < spacePoly.size(); k++){
				linePoints.push_back(spacePoly[k]);
			}
			// add the vector of points to the main vector (which is indeed a vector of vectors)
			points.push_back(linePoints);
		}
	}

	nextPointSpeed = 0.65; // speed at which the sketch cycles through the points

	// create the text outline
	outline.setStrokeColor(ofColor(0, 255, 255));
	outline.setStrokeWidth(15); // maximum determined by graphics card
	for(int i = 0; i < points.size(); i++){
		vector <ofVec2f> charPoints = points[i];
		outline.moveTo(charPoints[0]);
		for(int j = 1; j < charPoints.size(); j++){
			outline.lineTo(charPoints[j]);
		}
	}
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	if(saveOneFrame){
		ofBeginSaveScreenAsPDF("StringAsPoints-" + ofGetTimestampString() + ".pdf", false);
	}

	ofTranslate(center.x, center.y);

	// draw the complete text as a thick outline
	outline.draw(); // created in setup once

	// draw the extracted points as black circles
	ofFill();
	ofSetColor(0);
	for(int i = 0; i < points.size(); i++){
		vector <ofVec2f> charPoints = points[i];
		for(int j = 0; j < charPoints.size(); j++){
			ofCircle(charPoints[j], 2);
		}
	}

	// draw thin transparant lines between two points
	// dynamically set the 'opposite' point based on the current frame number
	ofNoFill();
	ofSetColor(0, 125);
	ofSetLineWidth(0.75);
	int fc = (int)(ofGetFrameNum() * nextPointSpeed);
	for(int i = 0; i < points.size(); i++){
		vector <ofVec2f> charPoints = points[i];
		for(int j = 0; j < charPoints.size(); j++){
			ofLine(charPoints[j], charPoints[(j + fc) % charPoints.size()]);
		}
	}

	if(saveOneFrame){
		ofEndSaveScreenAsPDF();
		saveOneFrame = false;
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == 's'){
		saveOneFrame = true; // set the variable to true to save a single frame as a PDF file / page
	}
}
