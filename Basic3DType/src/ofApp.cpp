#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(255);
	ofEnableAntiAliasing();
	ofEnableSmoothing();
	ofSetFrameRate(60);

	ofEnableDepthTest(); // enable depth test for correctly drawing 3D shapes

	bResample = true;
	resampleSpacing = 5.0;
	letterThickness = 50.0;

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

	colorFaces(mesh);

	// adhere easycam view to openFrameworks coordinate system
	cam.setVFlip(true);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	cam.begin();
	ofPushMatrix();

	ofTranslate(center.x, center.y);

	ofSetColor(0);
	mesh.disableColors();
	mesh.drawWireframe();

	mesh.enableColors();
	mesh.draw();

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
void ofApp::colorFaces(ofMesh & mesh){
	for(int i = 0; i < mesh.getVertices().size(); i++){
		const ofVec3f & v = mesh.getVertices()[i];
		ofColor col;
		col.setHsb(ofMap(v.x + v.y, -1000, 1000, 0, 255), 255, 255);
		mesh.addColor(col);
	}
}
