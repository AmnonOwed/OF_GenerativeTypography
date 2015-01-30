#include "Circle.h"

Circle::Circle(){
}

//--------------------------------------------------------------
void Circle::setup(int _x, int _y){
	x = _x;
	y = _y;

	radius = 0;
	maxRadius = ofRandom(3, 25);
	growRate = ofRandom(0.2, 2);

	isMaxRadius = false;
	isOverlap = false;

	c.setHsb((x + 2 * y + ofGetFrameNum()) % 255, 191.25, 216.75);
}

//--------------------------------------------------------------
void Circle::update(){
	if(!isMaxRadius && !isOverlap){
		radius += growRate;
		isMaxRadius = radius >= maxRadius;
	}
}

//--------------------------------------------------------------
void Circle::display(){
	ofFill();
	ofSetColor(c);
	ofEllipse(x, y, 2 * radius, 2 * radius);
	ofNoFill();
	ofSetColor(0);
	ofEllipse(x, y, 2 * radius, 2 * radius);
}

void Circle::overlap(const vector <Circle *> circles){
	if(!isMaxRadius && !isOverlap){
		for(int i = 0; i < circles.size(); i++){
			if(circles[i] != this){
				if(ofDist(x, y, circles[i]->x, circles[i]->y) <= circles[i]->radius + radius + growRate){
					isOverlap = true;
				}
			}
		}
	}
}
