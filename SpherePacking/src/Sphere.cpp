#include "Sphere.h"

//--------------------------------------------------------------
void Sphere::setPointers(ofMesh * _meshPtr, ofVec3f * _boundsMinPtr, ofVec3f * _boundsMaxPtr, vector <Sphere *> * _spheresPtr){
	meshPtr = _meshPtr;
	boundsMinPtr = _boundsMinPtr;
	boundsMaxPtr = _boundsMaxPtr;
	spheresPtr = _spheresPtr;
}

//--------------------------------------------------------------
void Sphere::setup(){
	setPosition(getPointInMesh());
	setRadius(0);
	growRate = ofRandom(0.2, 2);
	float rPos = ofMap(getX(), boundsMinPtr->x, boundsMaxPtr->x, 0, 255) + ofMap(getY(), boundsMinPtr->y, boundsMaxPtr->y, 0, 255);
	c.setHsb(fmod(rPos, 255), 255, ofMap(getZ(), boundsMinPtr->z, boundsMaxPtr->z, 40, 255));
}

//--------------------------------------------------------------
void Sphere::update(){
	if(!isOverlap){
		setRadius(getRadius() + growRate);
		checkOverlap();
	}
}

//--------------------------------------------------------------
void Sphere::draw(){
	ofSetColor(c);
	ofSpherePrimitive::draw();
}

//--------------------------------------------------------------
ofVec3f Sphere::getPointInMesh(){
	ofVec3f p = getRandomPointWithinBounds();
	while(!ofxPointInMesh::isInside(p, *meshPtr)){
		p = getRandomPointWithinBounds();
	}
	return p;
}

//--------------------------------------------------------------
ofVec3f Sphere::getRandomPointWithinBounds(){
	float x = ofRandom(boundsMinPtr->x, boundsMaxPtr->x);
	float y = ofRandom(boundsMinPtr->y, boundsMaxPtr->y);
	float z = ofRandom(boundsMinPtr->z, boundsMaxPtr->z);
	return ofVec3f(x, y, z);
}

//--------------------------------------------------------------
void Sphere::checkOverlap(){
	for(int i = 0; i < spheresPtr->size(); i++){
		Sphere * otherSphere = (*spheresPtr)[i];
		if(otherSphere != this){
			if(getPosition().distance(otherSphere->getPosition()) <= otherSphere->getRadius() + getRadius() + growRate){
				isOverlap = true;
			}
		}
	}
}
