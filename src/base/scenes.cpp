#include "scenes.h"
#include "ofMain.h"
#include "openGLstuff.h"
#include "ofxAssimpModelLoader.h"
#include <vector>
#include "misc.h"

void scene1::setup(){


	//setting our perspective and view matrix
	Projection_= ofMatrix4x4::newPerspectiveMatrix(45.0f, 1024.0 /768.0, 0.1f, 100.0f);
	//note1: openFrameworks builds the lookAt matrix weirdly, I use the same definition as glm
	View_ = newLookAtMatrixGLMway(ofVec3f(0,0,0),ofVec3f(1,0,0),ofVec3f(0,1,0));
	//note2: I've edited the function ofMatrix4x4 operator * ( const ofMatrix4x4 &m ) in ofMatrix4x4 to get this right
	Model_ = ofMatrix4x4::newIdentityMatrix();
	Map_ = ofMatrix4x4::newRotationMatrix(-5.0f,0,0,1);
	angle_[0]=0;angle_[1]=0;

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glGenVertexArrays(1, &VertexArrayID_);
	glBindVertexArray(VertexArrayID_);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glShadeModel(GL_SMOOTH);

	//-----------------------------initializing skybox-----------------------------------
	faces_.push_back("bin/data/sky_right.png");
	faces_.push_back("bin/data/sky_left.png");
	faces_.push_back("bin/data/sky_top.png");
	faces_.push_back("bin/data/sky_bottom.png");
	faces_.push_back("bin/data/sky_back.png");
	faces_.push_back("bin/data/sky_front.png");
	textures_[0]=configure_cubeMap(faces_);

	ofxAssimpModelLoader modelLoader;
	if(modelLoader.loadModel("cube.obj",false)){
		cubeMesh_=modelLoader.getMesh(0);
	}
	programID_[0]=loadShaders("bin/data/cubemap.vert","bin/data/cubemap.frag");
	textureLocations_[0] = glGetUniformLocation(programID_[0], "skybox");
	vertexBufferID_[0]=configure_array(cubeMesh_.getVertices()); flag_[0]=1;
	numVertices_[0] = cubeMesh_.getNumVertices();

	//---------------------------initializing airplane-----------------------------------
	if(modelLoader.loadModel("airplane2.obj",false)){
		airplaneMesh_=modelLoader.getMesh(0);
	}


	//tex_.load("bin/data/airplane_tex.png");
	programID_[1]=loadShaders("bin/data/shaderScene1.vert","bin/data/shaderScene1.frag");
	imageLoc_="bin/data/airplane_tex.png";
	ofImage image0; image0.load(imageLoc_);
	textures_[1]=configure_texture2D(image0);
	textureLocations_[1] = glGetUniformLocation(programID_[1], "texSampler");
	textureLocations_[2] = glGetUniformLocation(programID_[1], "skybox");


	flag_[1]=configure_object(airplaneMesh_, vertexBufferID_[1],normalBufferID_[0],uvBufferID_[0],indexBufferID_[0]);
	if(flag_[1] & 8){
		numVertices_[1]=airplaneMesh_.getNumIndices();
	}else{
		numVertices_[1] = airplaneMesh_.getNumVertices();
	}





}
void scene1::draw() {

	if(abs(angle_[0])>1.0){
		Map_= ofMatrix4x4::newRotationMatrix(-0.01f*angle_[0],0,0,1)*Map_;
	}
	if(abs(angle_[1])>1.0){
		Map_= Map_*ofMatrix4x4::newRotationMatrix(-0.01f*angle_[1],0,1,0);

	}

	GLuint mvp_handle;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_FALSE);
	glUseProgram(programID_[0]);
	draw_object(vertexBufferID_[0],vertexBufferID_[0], numVertices_[0], flag_[0]);
	ofMatrix4x4 mvp=Projection_*View_*Map_;
	mvp_handle = glGetUniformLocation(programID_[0], "PV");
	glUniformMatrix4fv(mvp_handle, 1, GL_FALSE, mvp.getPtr());
	draw_texture(textures_[0],textureLocations_[0],GL_TEXTURE_CUBE_MAP,0);

	glDepthMask(GL_TRUE);


	glUseProgram(programID_[1]);
	//draw_object(vertexBufferID_[1],uvBufferID_, numVertices_[1],flag_[1]);
	draw_object(vertexBufferID_[1],uvBufferID_[0],  normalBufferID_[0],indexBufferID_[0], numVertices_[1],flag_[1]);

	mvp_handle = glGetUniformLocation(programID_[1], "MVP");
	ofMatrix4x4 translate = ofMatrix4x4::newTranslationMatrix(0.5,-0.1,0);
	ofMatrix4x4 rotate = ofMatrix4x4::newRotationMatrix(-90.0,0,1,0);
	ofMatrix4x4 scale = ofMatrix4x4::newScaleMatrix(0.1,0.1,0.1);
	mvp=Projection_*View_*translate*scale*rotate*Model_;
	glUniformMatrix4fv(mvp_handle, 1, GL_FALSE, mvp.getPtr());
	draw_texture(textures_[1],textureLocations_[1],GL_TEXTURE_2D,0);

	mvp_handle = glGetUniformLocation(programID_[1], "TexMat");
	mvp=Map_.getInverse()*translate*scale*rotate*Model_;
	glUniformMatrix4fv(mvp_handle, 1, GL_FALSE, mvp.getPtr());
	draw_texture(textures_[0],textureLocations_[2],GL_TEXTURE_CUBE_MAP,1);

	mvp_handle = glGetUniformLocation(programID_[1], "VM");
	mvp=View_*translate*scale*rotate*Model_;
	glUniformMatrix4fv(mvp_handle, 1, GL_FALSE, mvp.getPtr());


}

void scene1::keyPressed(int key){
	if(key==357)
	{
		if(angle_[0]<10.0){
			angle_[0]+=0.5;

		}
		//Map_= ofMatrix4x4::newRotationMatrix(0.5f,0,0,1)*Map_;
	}
	if(key==359){
		if(angle_[0]>-20.0){
			angle_[0]-=0.5;
		}
		//Map_= ofMatrix4x4::newRotationMatrix(-0.5f,0,0,1)*Map_;
	}
	if(key==356){

		if(angle_[1]<30.0){
			angle_[1]+=0.5;
		}
		//Map_= Map_*ofMatrix4x4::newRotationMatrix(0.5f,0,1,0);
	}
	if(key==358){
		if(angle_[1]>-30.0){
			angle_[1]-=0.5;
		}
		//Model_.rotate(0.5f,0,0,1);
		//Map_= Map_*ofMatrix4x4::newRotationMatrix(-0.5f,0,1,0);
	}
	if(key==114){
		Model_.rotate(-0.5f,1,0,0);
	}
	if(key==116){
		Model_.rotate(0.5f,1,0,0);
	}
	if(key==117){
		update_cubeMap(faces_,textures_[0]);
		ofImage image0; image0.load(imageLoc_);
		update_texture2D(textures_[1], image0);
	}

	Model_=ofMatrix4x4::newRotationMatrix(angle_[0],1,0,0)*ofMatrix4x4::newRotationMatrix(angle_[1],0,0,1);


}

