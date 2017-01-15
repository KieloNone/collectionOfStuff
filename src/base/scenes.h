#pragma once
#include "ofMain.h"


class scene1 {
public:
	scene1(){}
	~scene1(){}

	void setup();
	void draw() ;
	void keyPressed(int key);
private:
	GLuint programID_[3];
	GLuint textures_[2];
	GLuint textureLocations_[4];

	GLuint VertexArrayID_;
	GLuint vertexBufferID_[3];
	GLuint uvBufferID_[2];
	GLuint indexBufferID_[2];
	GLuint normalBufferID_[2];

	int numVertices_[3];
	int flag_[3];
	ofMesh cubeMesh_;
	ofMesh airplaneMesh_;
	ofMesh planeMesh_;

	ofMatrix4x4 Projection_;
	ofMatrix4x4 View_;
	ofMatrix4x4 Model_;
	ofMatrix4x4 Map_;
	vector<const GLchar*> faces_;
	const GLchar* imageLoc_;

	float angle_[2];

};




