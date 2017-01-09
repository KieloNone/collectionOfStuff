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
	GLuint m_VertexArrayID;
	GLuint m_vertexBufferID;
	GLuint m_vertexBufferID2;
	GLuint m_uvBufferID;
	GLuint m_colorBufferID;
	GLuint m_indexBufferID;
	GLuint m_programID[2];
	GLuint m_textures[3];
	GLuint m_textureLocations[3];
	int m_numVertices;
	int m_flag[2];
	float angle;
	ofMesh ourMesh;
	ofMesh ourMesh2;
	ofMatrix4x4 m_PV;
	ofMatrix4x4 m_Model;
};

GLuint configure_texture2D(ofImage image);
int configure_object(ofMesh & mesh, GLuint& vertexBufferID,GLuint& colorBufferID,GLuint& uvBufferID,GLuint& indexBufferID);
void draw_object(GLuint vertexBufferID,GLuint uvBufferID, int numVertices,int flag);
GLuint loadShaders(const char * vertex_file_path,const char * fragment_file_path);
void draw_texture(GLuint texture,GLuint textureID, GLuint type, GLuint position);
GLuint configure_cubeMap(vector<const GLchar*> & faces);

static const GLfloat g_vertex_buffer_data[] = {
		-1.0,-1.0,-1.0,
		-1.0,-1.0,0.0,
		0.0,1.0,0.0

};
