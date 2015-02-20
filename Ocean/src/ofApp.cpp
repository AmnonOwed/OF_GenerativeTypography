#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	saveOneFrame = false; // variable used to save a single frame as a PDF page
	bReset = true; // variable used to reset the application

	ofBackground(255);
	ofEnableAntiAliasing();
	ofEnableSmoothing();
	ofSetFrameRate(60);

	ofNoFill();

	ofTrueTypeFont ttf;
	// to extract points, loadFont must be called with makeContours set to true
	ttf.loadFont(OF_TTF_SANS, 300, true, false, true);
	string s = "TYPE";

	// Center string code from:
	// https://github.com/armadillu/ofxCenteredTrueTypeFont/blob/master/src/ofxCenteredTrueTypeFont.h
	ofRectangle r = ttf.getStringBoundingBox(s, 0, 0);
	ofVec2f center = ofVec2f(floor(-r.x - r.width * 0.5f), floor(-r.y - r.height * 0.5f));
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
			ofPolyline spacePoly = polylines[j].getResampledBySpacing(5);
			// add all the points of the resampled polyline to the character vector
			for(int k = 0; k < spacePoly.size(); k++){
				spacePoly[k] += center;
				linePoints.push_back(spacePoly[k]);
			}
			// add the vector of points to the main vector (which is indeed a vector of vectors)
			points.push_back(linePoints);
		}
	}
	// make a copy of the points to use when resetting the application
	pointsCopy = points;

	// wave boundaries (optimized for graphical effect and a 1280 x 720 canvas)
	minX = 0;
	maxX = 1500;
	minY = -500;
	maxY = 1000;
}

//--------------------------------------------------------------
void ofApp::update(){
	if(bReset){
		reset();
	}
	else{
		// move the background lines
		for(int i = 0; i < lines.size(); i++){
			lines[i].update();
		}
		// move the letters
		for(int i = 0; i < points.size(); i++){
			vector <ofVec2f> & charPoints = points[i];
			for(int j = 0; j < charPoints.size(); j++){
				ofVec2f & p = charPoints[j];
				p += getVelocity(p);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	if(saveOneFrame){
		ofBeginSaveScreenAsPDF("Ocean-" + ofGetTimestampString() + ".pdf", false);
	}

	// display the background lines
	ofSetLineWidth(0.65);
	ofSetColor(95);
	for(int i = 0; i < lines.size(); i++){
		lines[i].display(minX, maxX);
	}

	// display the letters
	ofNoFill();
	ofSetLineWidth(2);
	ofSetColor(0);
	for(int i = 0; i < points.size(); i++){
		vector <ofVec2f> & charPoints = points[i];
		ofBeginShape();
		for(int j = 0; j < charPoints.size(); j++){
			ofCurveVertex(charPoints[j].x, charPoints[j].y);
		}
		ofEndShape(true);
	}

	if(saveOneFrame){
		ofEndSaveScreenAsPDF();
		saveOneFrame = false;
	}
}

//--------------------------------------------------------------
void ofApp::reset(){
	points = pointsCopy;

	lines.clear();

	vector <ofVec2f> firstLineVecs;
	float d12 = (maxX - minX) / 12.0;
	float d6 = (maxX - minX) / 6.0;
	float x = minX;
	while(x < maxX){
		ofVec2f v(x += ofRandom(d12, d6), maxY);
		firstLineVecs.push_back(v);
	}
	MovingLine firstLine;
	firstLine.setup(firstLineVecs);
	lines.push_back(firstLine);

	MovingLine previousLine = lines.back();
	while(!previousLine.isFullyOutsideBoundaries(minY)){
		ofVec2f moveSpeed(ofRandom(-2, 3), ofRandom(-8, 2));
		vector <ofVec2f> newLineVecs;
		for(int i = 0; i < previousLine.vertices.size(); i++){
			ofVec2f v(previousLine.vertices[i].x + moveSpeed.x, previousLine.vertices[i].y + moveSpeed.y);
			newLineVecs.push_back(v);
		}
		newLineVecs[0].x = MIN(newLineVecs[0].x, minX);
		MovingLine newLine;
		newLine.setup(newLineVecs);
		lines.push_back(newLine);
		previousLine = lines.back();
	}

	bReset = false;
}

//--------------------------------------------------------------
// flowfield to generate the velocities for the points in the text
ofVec2f ofApp::getVelocity(const ofVec2f & pos){
	float factor = 0.005;
	float angle = ofNoise(pos.x * factor, pos.y * factor, ofGetFrameNum() * factor) * 360;
	ofVec2f vel(1, 0);
	vel.rotate(angle);
	vel.limit(0.65); // maximize the movement speed
	return vel;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == ' '){
		bReset = true; // set the variable to true to reset the application
	}
	if(key == 's'){
		saveOneFrame = true; // set the variable to true to save a single frame as a PDF file / page
	}
}
