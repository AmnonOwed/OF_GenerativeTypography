#include "MovingLine.h"
#include "ofApp.h"

MovingLine::MovingLine(){
}

//--------------------------------------------------------------
void MovingLine::setup(vector <ofVec2f> _vertices, ofApp * _ptrOfApp){
	vertices = _vertices;
	ptrOfApp = _ptrOfApp;
}

//--------------------------------------------------------------
void MovingLine::update(){
	for(int i = 0; i < vertices.size(); i++){
		ofVec2f & v = vertices[i];
		v += ptrOfApp->getVelocity(v);
	}
	vertices[0].x = 0;
}

//--------------------------------------------------------------
void MovingLine::display(){
	ofBeginShape();
	ofCurveVertex(ptrOfApp->minX, vertices[0].y);
	for(int i = 0; i < vertices.size(); i++){
		ofCurveVertex(vertices[i].x, vertices[i].y);
	}
	ofCurveVertex(ptrOfApp->maxX, vertices.back().y);
	ofEndShape();
}

//--------------------------------------------------------------
// check if the line is completely above the upper boundary (then it will return true)
bool MovingLine::isFullyOutsideBoundaries(){
	for(int i = 0; i < vertices.size(); i++){
		if(vertices[i].y > ptrOfApp->minY){
			return false;
		}
	}
	return true;
}
