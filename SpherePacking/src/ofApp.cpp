#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofEnableAntiAliasing();
	ofEnableSmoothing();
	ofEnableDepthTest();

	bResample = true;
	resampleSpacing = 5.0;
	letterThickness = 145.0;

	ofTrueTypeFont ttf;
	// to extract points, loadFont must be called with makeContours set to true
	ttf.loadFont(OF_TTF_SANS, 300, true, false, true);
	string s = "TYPE";

	// Center string code from:
	// https://github.com/armadillu/ofxCenteredTrueTypeFont/blob/master/src/ofxCenteredTrueTypeFont.h
	ofRectangle r = ttf.getStringBoundingBox(s, 0, 0);
	center = ofVec2f(floor(-r.x - r.width * 0.5f), floor(-r.y - r.height * 0.5f));

	vector <ofTTFCharacter> letters = ttf.getStringAsPoints(s);

	for(int i = 0; i < letters.size(); i++){
		ofPath resampledPath = bResample ? resamplePath(letters[i], resampleSpacing) : letters[i];
		ofMesh meshLetter = createMeshFromPath(resampledPath, letterThickness);
		mesh.append(meshLetter);
	}

	getBoundaries(mesh);

	numSpheres = 7000; // the maximum number of Spheres
	addSpeed = 25; // the number of Spheres that is added per update() loop

	ofEnableLighting();
	ofSetGlobalAmbientColor(ofColor(128));

	directionalLight.setDiffuseColor(ofColor(128));
	directionalLight.setPosition(boundsMax);
	directionalLight.enable();

	cam.setVFlip(true);
}

//--------------------------------------------------------------
void ofApp::update(){
	if(spheres.size() < numSpheres){
		for(int i = 0; i < addSpeed; i++){
			Sphere * s = new Sphere();
			s->setPointers(&mesh, &boundsMin, &boundsMax, &spheres);
			s->setup();
			spheres.push_back(s);
		}
	}
	for(int i = 0; i < spheres.size(); i++){
		spheres[i]->update();
	}
	ofSetWindowTitle("numSpheres: " + ofToString(spheres.size()) + " of " + ofToString(numSpheres) + (spheres.size() == numSpheres ? " (completed)" : " (may be sluggish while growing Spheres)"));
}

//--------------------------------------------------------------
void ofApp::draw(){
	cam.begin();
	ofPushMatrix();

	ofBackground(0);

	ofTranslate(center.x, center.y);

	for(int i = 0; i < spheres.size(); i++){
		spheres[i]->draw();
	}

	ofPopMatrix();
	cam.end();
}

//--------------------------------------------------------------
ofPath ofApp::resamplePath(ofPath path, float spacing){
	ofPath resampledPath;
	vector <ofPolyline> polylines = path.getOutline();
	for(int j = 0; j < polylines.size(); j++){
		ofPolyline spacePoly = polylines[j].getResampledBySpacing(spacing);
		resampledPath.moveTo(spacePoly[0]);
		for(int k = 1; k < spacePoly.size(); k++){
			resampledPath.lineTo(spacePoly[k]);
		}
		resampledPath.lineTo(spacePoly[0]);
	}
	return resampledPath;
}

//--------------------------------------------------------------
ofMesh ofApp::createMeshFromPath(ofPath path, float thickness){
	ofMesh mesh;
	// get outline of original path (for sides)
	vector <ofPolyline> outline = path.getOutline();
	// add front to mesh
	mesh.append(path.getTessellation());
	// get number of vertices in original path
	int numVerts = mesh.getNumVertices();
	// define offset based on thickness
	ofVec3f offset = ofVec3f(0, 0, -thickness);
	// translate path
	path.translate(offset);
	// add back to mesh
	mesh.append(path.getTessellation());
	// add sides to mesh (via indices)
	for(int j = 0; j < outline.size(); j++){
		int outlineSize = outline[j].getVertices().size();
		vector <int> outlineIndices;
		for(int i = 1; i < outlineSize; i++){
			ofVec3f & v = outline[j].getVertices()[i];
			int originalIndex = -1;
			for(int k = 0; k < numVerts; k++){
				if(v.match(mesh.getVertices()[k])){
					originalIndex = k;
					break;
				}
			}
			outlineIndices.push_back(originalIndex);
		}
		for(int i = 0; i < outlineIndices.size(); i++){
			const int a = outlineIndices[i];
			const int b = outlineIndices[(i + 1) % outlineIndices.size()];
			const int c = outlineIndices[i] + numVerts;
			const int d = outlineIndices[(i + 1) % outlineIndices.size()] + numVerts;
			mesh.addTriangle(a, b, c);
			mesh.addTriangle(c, b, d);
		}
	}
	return mesh;
}

//--------------------------------------------------------------
void ofApp::getBoundaries(ofMesh & mesh){
	vector <ofVec3f> & vertices = mesh.getVertices();
	for(int i = 0; i < vertices.size(); i++){
		ofVec3f & v = vertices[i];
		if(v.x < boundsMin.x){
			boundsMin.x = v.x;
		}
		if(v.y < boundsMin.y){
			boundsMin.y = v.y;
		}
		if(v.z < boundsMin.z){
			boundsMin.z = v.z;
		}
		if(v.x > boundsMax.x){
			boundsMax.x = v.x;
		}
		if(v.y > boundsMax.y){
			boundsMax.y = v.y;
		}
		if(v.z > boundsMax.z){
			boundsMax.z = v.z;
		}
	}
}
