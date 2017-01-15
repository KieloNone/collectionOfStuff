#pragma once

#include "ofMain.h"

/*
 * openGLstuff.h
 * This is a dummy class meant for studying openGL.
 * Most of the stuff done here is implemented in openFrameworks with better errorhandling etc.
 * Therefore there should be no reason to use this class except if you do something that openFrameworks does not support
 */

class openGLexample {
public:
	void setup();
	void draw();
private:
	GLuint VertexArrayID_;
	GLuint vertexBufferID_;
	GLuint vertexBufferID2_;
	GLuint uvBufferID_;
	GLuint colorBufferID_;
	GLuint indexBufferID_;
	GLuint programID_[2];
	GLuint textures_[3];
	GLuint textureLocations_[3];
	int numVertices_;
	int flag_[2];
	float angle_;
	ofMesh ourMesh_;
	ofMesh ourMesh2_;
	ofMatrix4x4 PV_;
	ofMatrix4x4 Model_;
};

template <class T>
GLuint configure_array(vector<T> & data);

GLuint configure_texture2D(ofImage image);
void update_texture2D(GLuint textureID, ofImage image);

int configure_object(ofMesh & mesh, GLuint& vertexBufferID,GLuint& colorBufferID,GLuint& uvBufferID,GLuint& indexBufferID);

GLuint loadShaders(const char * vertex_file_path,const char * fragment_file_path);
void draw_texture(GLuint texture,GLuint textureID, GLuint type, GLuint position);
GLuint configure_cubeMap(vector<const GLchar*> & faces);
void update_cubeMap(vector<const GLchar*> & faces,GLuint & textureID);

void draw_object(GLuint vertexBufferID,GLuint uvBufferID, int numVertices,int flag);
void draw_object(GLuint vertexBufferID,GLuint  uvBufferID, GLuint normalBufferID, GLuint indexBufferArray, int numIndices, int flag);

static const GLfloat g_vertex_buffer_data[] = {
		-1.0,-1.0,-1.0,
		-1.0,-1.0,0.0,
		0.0,1.0,0.0

};

