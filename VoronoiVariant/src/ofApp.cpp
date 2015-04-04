#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	saveOneFrame = false; // variable used to save a single frame as a PDF page
	reset = true; // variable used to reset/regenerate the voronoi shape

	ofBackground(255);
	ofEnableAntiAliasing();
	ofEnableSmoothing();
	ofSetFrameRate(60);

	ofTrueTypeFont ttf;
	// to extract points, loadFont must be called with makeContours set to true
	ttf.loadFont(OF_TTF_SANS, 275, true, false, true);
	string s = "TYPE";

	// Center string code from:
	// https://github.com/armadillu/ofxCenteredTrueTypeFont/blob/master/src/ofxCenteredTrueTypeFont.h
	ofRectangle r = ttf.getStringBoundingBox(s, 0, 0);
	ofVec2f center = ofVec2f(floor(-r.x - r.width * 0.5f), floor(-r.y - r.height * 0.5f));
	center.x += ofGetWidth() / 2;
	center.y += ofGetHeight() / 2;

	// get the string as paths, place all the text points in a vector
	vector <ofTTFCharacter> paths = ttf.getStringAsPoints(s);
	for(int i = 0; i < paths.size(); i++){
		// for every character break it out to polylines
		vector <ofPolyline> polylines = paths[i].getOutline();
		// for every polyline...
		for(int j = 0; j < polylines.size(); j++){
			// resample polyline to enforce equal spacing between points
			ofPolyline spacePoly = polylines[j].getResampledBySpacing(25);
			// add all the points of the resampled polyline to the textPoints vector
			for(int k = 0; k < spacePoly.size(); k++){
				// add center to each point to centralize the text placement
				textPoints.push_back(spacePoly[k] + center);
			}
		}
	}

	fbo_color = ofColor(0);
	ofFbo fbo;
	fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	pix.allocate(ofGetWidth(), ofGetHeight(), OF_PIXELS_RGBA);
	fbo.begin();
	ofClear(0, 0, 0, 0);
	ofSetColor(fbo_color);
	ttf.drawString(s, center.x, center.y);
	fbo.end();

	fbo.readToPixels(pix); // the ofPixels class has a convenient getColor() method
}

//--------------------------------------------------------------
void ofApp::update(){
	if(reset){
		// clear voronoi
		voronoi.clear();

		// add text points to the voronoi
		voronoi.addPoints(textPoints);

		// add random points to the voronoi
		vector <ofPoint> randomPoints = generateRandomPoints(225);
		voronoi.addPoints(randomPoints);

		// set the bounds of the voronoi
		ofRectangle bounds = ofRectangle(0, 0, ofGetWidth(), ofGetHeight());
		voronoi.setBounds(bounds);

		// generate the voronoi
		voronoi.generate();

		// store which cells/points are in the text
		vector <ofxVoronoiCell> cells = voronoi.getCells();
		insideText.clear();
		for(int i = 0; i < cells.size(); i++){
			bool inside = isPointInList(cells[i].pt, textPoints);
			insideText.push_back(inside);
		}

		reset = false;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	if(saveOneFrame){
		ofBeginSaveScreenAsPDF("VoronoiVariant-" + ofGetTimestampString() + ".pdf", false);
	}

	// scale the inner voronoi region shape based on the mouseX
	float scaleFactor = ofMap(ofClamp(ofGetMouseX(), 0, ofGetWidth()), 0, ofGetWidth(), 0, 1.15);

	// draw cells
	vector <ofxVoronoiCell> cells = voronoi.getCells();
	for(int i = 0; i < cells.size(); i++){
		if(insideText[i]){
			vector <ofPoint> & cellPoints = cells[i].pts;

			// draw a straight voronoi region outline
			ofPath outline = ofPath();
			outline.setFilled(false);
			outline.setStrokeColor(ofColor(0));
			outline.setStrokeWidth(0.85);
			for(int j = 0; j < cellPoints.size(); j++){
				ofPoint & thisPt = cellPoints[j];
				if(j == 0){
					outline.moveTo(thisPt);
				}
				else{
					outline.lineTo(thisPt);
				}
			}
			outline.close();
			outline.draw();

			// calculate the center of the voronoi region
			ofPoint center;
			for(int j = 0; j < cellPoints.size(); j++){
				ofPoint & thisPt = cellPoints[j];
				center += thisPt;
			}
			center /= cellPoints.size();

			// draw a curved, colored and scaled voronoi region
			ofPath path = ofPath();
			ofSeedRandom(i * 1000000);
			ofColor fill_color;
			fill_color.setHsb(ofRandom(127.5, 165.75), ofRandom(89.25, 191.25), ofRandom(89.25, 216.75));
			path.setFillColor(fill_color);
			path.setStrokeColor(ofColor(125));
			path.setStrokeWidth(0.75);
			path.curveTo(cellPoints.back()); // control point
			for(int j = 0; j < cellPoints.size(); j++){
				path.curveTo(cellPoints[j]);
			}
			path.curveTo(cellPoints[0]); // control point
			path.close();
			path.translate(-center);
			path.scale(scaleFactor, scaleFactor);
			path.translate(center);
			path.draw();

		}
	}

	if(saveOneFrame){
		ofEndSaveScreenAsPDF();
		saveOneFrame = false;
	}
}

//--------------------------------------------------------------
vector <ofPoint> ofApp::generateRandomPoints(int count){
	vector <ofPoint> points;
	for(int i = 0; i < count; i++){
		ofPoint newPoint = ofPoint(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
		while(pix.getColor(newPoint.x, newPoint.y) == fbo_color){
			newPoint = ofPoint(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
		}
		points.push_back(newPoint);
	}
	return points;
}

//--------------------------------------------------------------
bool ofApp::isPointInList(const ofPoint & point, const vector <ofPoint> & pointList){
	for(int i = 0; i < pointList.size(); i++){
		if(point == pointList[i]){
			return true;
		}
	}
	return false;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == 's'){
		saveOneFrame = true; // set the variable to true to save a single frame as a PDF file / page
	}
	if(key == ' '){
		reset = true; // set the variable to true to reset/regenerate the voronoi shape
	}
}
