#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	displayPoints = true; // variable used to toggle the display of points
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
			ofPolyline spacePoly = polylines[j].getResampledBySpacing(6);
			// add all the points of the resampled polyline to the textPoints vector
			for(int k = 0; k < spacePoly.size(); k++){
				// add center to each point to centralize the text placement
				textPoints.push_back(spacePoly[k] + center);
			}
		}
	}

	currentPoint = 0; // holds the current point used for the 'moving points' effect
	interpolationBetweenPoints = 0; // holds the interpolation value used for the 'moving points' effect

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
	if(displayPoints){
		// global variables for the 'moving points' effect
		interpolationBetweenPoints += 0.01;
		if(interpolationBetweenPoints >= 1){
			interpolationBetweenPoints = 0;
			currentPoint++;
		}
	}
	if(reset){
		// clear voronoi
		voronoi.clear();

		// add text points to the voronoi
		voronoi.addPoints(textPoints);

		// add random points to the voronoi
		vector <ofPoint> randomPoints = generateRandomPoints(250);
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
		ofBeginSaveScreenAsPDF("VoronoiType-" + ofGetTimestampString() + ".pdf", false);
	}

	// draw cells
	vector <ofxVoronoiCell> cells = voronoi.getCells();
	for(int i = 0; i < cells.size(); i++){
		if(insideText[i]){
			vector <ofPoint> & cellPoints = cells[i].pts;
			ofPath path = ofPath();
			for(int j = 0; j < cellPoints.size(); j++){
				ofPoint & thisPt = cellPoints[j];
				if(j == 0){
					ofColor fill_color;
					fill_color.setHsb(fmod(i * 157.59, 255), 255, 255);
					path.setFillColor(fill_color);
					path.setStrokeColor(ofColor(0));
					path.setStrokeWidth(0.85);
					path.moveTo(thisPt);
				}
				else{
					path.lineTo(thisPt);
				}
			}
			path.close();
			path.draw();
		}
	}

	// draw moving point in each cell (if boolean is true)
	if(displayPoints){
		ofFill();
		ofSetColor(0);
		for(int i = 0; i < cells.size(); i++){
			if(insideText[i]){
				vector <ofPoint> & cellPoints = cells[i].pts;
				float interpolationOfPoints = currentPoint % cellPoints.size() / float(cellPoints.size());
				int start = (int(interpolationOfPoints * cellPoints.size()) + i) % cellPoints.size();
				int end = (start + 1) % cellPoints.size();
				ofPoint interpolatedPoint = cellPoints[start].getInterpolated(cellPoints[end], interpolationBetweenPoints);
				ofCircle(interpolatedPoint, 2);
			}
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
	if(key == 'p'){
		displayPoints = !displayPoints; // toggle display of points
	}
	if(key == 's'){
		saveOneFrame = true; // set the variable to true to save a single frame as a PDF file / page
	}
	if(key == ' '){
		reset = true; // set the variable to true to reset/regenerate the voronoi shape
	}
}
