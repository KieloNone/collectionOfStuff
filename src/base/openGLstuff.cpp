#include "openGLstuff.h"
#include "ofMain.h"
#include "misc.h"
#include "ofxAssimpModelLoader.h"

void openGLexample::setup(){

	//loading some textures
	ofImage image0;
	ofImage image1;
	image0.load("bin/data/kissa.png");
	image1.load("bin/data/koira.png");
	//

	//data fir cubemap
	vector<const GLchar*> faces;
	faces.push_back("bin/data/koira.png");
	faces.push_back("bin/data/koira.png");
	faces.push_back("bin/data/koira.png");
	faces.push_back("bin/data/koira.png");
	faces.push_back("bin/data/koira.png");
	faces.push_back("bin/data/koira.png");


	//loading a cube, using motorsaw when a knife would have been enough
	ofxAssimpModelLoader modelLoader;
	if(modelLoader.loadModel("cube.obj",false)){
		ourMesh=modelLoader.getMesh(0);
	}

	ourMesh2.addVertices(ourMesh.getVertices());

	//getting the appropriate matrixes
	ofMatrix4x4 Projection= ofMatrix4x4::newPerspectiveMatrix(45.0f, 1024.0 /768.0, 0.1f, 100.0f);

	//note1: openFrameworks builds the lookAt matrix weirdly, I use the same definition as glm
	ofMatrix4x4 View = newLookAtMatrixGLMway(ofVec3f(4,3,-3),ofVec3f(0,0,0),ofVec3f(0,1,0));

	//note2: The right order for multiplication is projection*view*model, but openFrameworks had overloaded the function * backwards!
	m_PV=View*Projection; //should be Projection*View...
	m_Model = ofMatrix4x4::newIdentityMatrix();

	//-------------------- openGL stuff begins here --------------------------------------------------------------------------------

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	//not sure what the following does but it is necessary:
	glGenVertexArrays(1, &m_VertexArrayID);
	glBindVertexArray(m_VertexArrayID);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);




	//creating shaders
	m_programID[0]=loadShaders("bin/data/shader.vert","bin/data/shader.frag");
	m_programID[1]=loadShaders("bin/data/cubemap.vert","bin/data/cubemap.frag");

	//creating textures
	m_textures[0]=configure_texture2D(image0);
	m_textures[1]=configure_texture2D(image1);
	m_textures[2]=configure_cubeMap(faces);

	//our shader has texture samplers "kissa and koira". The following get's their locations
	m_textureLocations[0] = glGetUniformLocation(m_programID[0], "kissa");
	m_textureLocations[1] = glGetUniformLocation(m_programID[0], "koira");
	m_textureLocations[2] = glGetUniformLocation(m_programID[1], "skybox");

	//creating our object, currently just creates vertex and uv buffers...
	m_flag[0]=configure_object(ourMesh, m_vertexBufferID,m_colorBufferID,m_uvBufferID,m_indexBufferID);
	m_numVertices = ourMesh.getNumVertices();
	m_flag[1]=configure_object(ourMesh2, m_vertexBufferID2,m_colorBufferID,m_colorBufferID,m_indexBufferID);
	m_numVertices = ourMesh.getNumVertices();
}

void openGLexample::draw(){

	GLuint mvp_handle;
	//we need to clear the color and depth buffers to avoid artifacts
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//telling GPU which shader program to use
	glDepthMask(GL_FALSE);
	glUseProgram(m_programID[1]);
	draw_object(m_vertexBufferID2,m_vertexBufferID2, m_numVertices,m_flag[1]);
    mvp_handle = glGetUniformLocation(m_programID[1], "PV");
    glUniformMatrix4fv(mvp_handle, 1, GL_FALSE, m_PV.getPtr());
	draw_texture(m_textures[2],m_textureLocations[2],GL_TEXTURE_CUBE_MAP,0);


	glDepthMask(GL_TRUE);

    glUseProgram(m_programID[0]);

    //telling GPU to draw our vertices
    draw_object(m_vertexBufferID,m_uvBufferID, m_numVertices,m_flag[0]);

    // sending matrix "mvp" to shader location "uniform mat4 MVP"
    // also adding some rotation in model space
    mvp_handle = glGetUniformLocation(m_programID[0], "MVP");
    m_Model.rotate(0.1f,0,1,0);
    ofMatrix4x4 mvp=m_Model*m_PV;
    glUniformMatrix4fv(mvp_handle, 1, GL_FALSE, mvp.getPtr());

    //sending textures to GPU
    draw_texture(m_textures[0],m_textureLocations[0],GL_TEXTURE_2D,0);
    draw_texture(m_textures[1],m_textureLocations[1],GL_TEXTURE_2D,1);
}

GLuint configure_texture2D(ofImage image){

	//getting image data from ofImage
	unsigned char * data=image.getPixels().getData();
	int width = image.getWidth();
	int height = image.getHeight();

    GLuint textureID;
    //creating a texture pointer at address textureID
    glGenTextures(1, &textureID);
    //bind the generated texture to GL_TEXTURE_2D variable
    glBindTexture(GL_TEXTURE_2D, textureID);
    //create the texture (allocating space, selecting type etc.)
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //setting what to do when the area to draw the texture is either bigger of smaller than the actual texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //linear interpolation when zooming out
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //mipmaps when zooming in
    glGenerateMipmap(GL_TEXTURE_2D); //generating the mipmaps

    //unbind GL_TEXTURE_2D
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;

}

GLuint configure_cubeMap(vector<const GLchar*> & faces){

	//tutorial for this: https://learnopengl.com/?_escaped_fragment_=Advanced-OpenGL/Cubemaps#!Advanced-OpenGL/Cubemaps
	//this function is pretty similar to that of configure_texture2D with the exeption that we need to load 6 images (one for each face)

	GLuint textureID;
	//generating texture pointer
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	ofImage image;
	int width;
	int height;
	unsigned char * data;
	for(GLuint i = 0; i < faces.size(); i++){
		image.load(faces[i]);
		width=image.getWidth();
		height=image.getHeight();
		data=image.getPixels().getData();
	    glTexImage2D(
	            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
	            GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	    }

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//what to do when the texture coordinates go out of bounds
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // one addition dimension
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return textureID;

}

int configure_object(ofMesh & mesh, GLuint& vertexBufferID,GLuint& colorBufferID,GLuint& uvBufferID,GLuint& indexBufferID){

	int count=0;

	//transmitting vertices to GPU
	if(mesh.getNumVertices()>0){
		// Generate 1 buffer, put the resulting identifier in vertexBufferID
		glGenBuffers(1, &vertexBufferID);
		// bind generated buffer to GL_ARRAY_BUFFER
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
		//create content
		glBufferData(GL_ARRAY_BUFFER, mesh.getNumVertices()*sizeof(ofVec3f) , mesh.getVerticesPointer(), GL_STATIC_DRAW);

		count=1;


	}
	/*
	if(mesh.getNumColors()>0){
		glGenBuffers(1, &uvBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
		glBufferData(GL_ARRAY_BUFFER, mesh.getNumColors()*sizeof(float) , mesh.getColorsPointer(), GL_STATIC_DRAW);
		count+=4;
	}*/
	if(mesh.getNumTexCoords()>0){
		glGenBuffers(1, &uvBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
		glBufferData(GL_ARRAY_BUFFER, mesh.getNumTexCoords()*sizeof(ofVec2f) , mesh.getTexCoordsPointer(), GL_STATIC_DRAW);

		count+=4;
	}

	//unbind buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*
	if(mesh.getNumIndices()){
		glGenBuffers(1, &indexBufferID);
		// note that we need GL_ELEMENT_ARRAY_BUFFER!
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.getNumIndices()*sizeof(unsigned int) , mesh.getIndexPointer(), GL_STATIC_DRAW); // Give our vertices to OpenGL.
		count+=8;
	}*/
	return count;
}


void draw_object(GLuint vertexBufferID,GLuint  uvBufferID, int numVertices, int flag){
	glEnableVertexAttribArray(0); // 1rst attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
	);
	//inserting uv to the gpu
	if(flag & 4){
		glEnableVertexAttribArray(1); //layout=1
		glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
		glVertexAttribPointer(
				1,                                // attribute.
				2,                                // size 2!!!!!!!!!!!!!!!!
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
		);
	}
	glDrawArrays(GL_TRIANGLES, 0, numVertices);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void draw_texture(GLuint texture,GLuint textureID, GLuint type, GLuint position){
	// activating the texture number <position>
    glActiveTexture(GL_TEXTURE0 + position);
    // binding our texture to context
    glBindTexture(type,texture);
    // saying that texture number <position> should be bound to variable textureID
    glUniform1i(textureID,position);

}

//the following 2 functions are slightly modified version of the original found from http://www.opengl-tutorial.org/

GLuint createAndCompileShader(const char * file_path, GLuint type){

	//create shader
	GLuint ShaderID = glCreateShader(type);

	// Read the shader code from the file
	std::string ShaderCode;
	std::ifstream ShaderStream(file_path, std::ios::in);
	if(ShaderStream.is_open()){
		std::string Line = "";
		while(getline(ShaderStream, Line))
			ShaderCode += "\n" + Line;
		ShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", file_path);
		getchar();
		return 0;
	}

	// Compile  Shader
	printf("Compiling shader : %s\n", file_path);
	char const * ShaderSourcePointer = ShaderCode.c_str();
	glShaderSource(ShaderID, 1, &ShaderSourcePointer , NULL);
	glCompileShader(ShaderID);

	// check whether or not the compilation was successful
	GLint Result = GL_FALSE;
	int InfoLogLength;

	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		if(type==GL_VERTEX_SHADER){
			std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
			glGetShaderInfoLog(ShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
			printf("%s\n", &VertexShaderErrorMessage[0]);
		}
		else{
			std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
			glGetShaderInfoLog(ShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
			printf("%s\n", &FragmentShaderErrorMessage[0]);
		}
	}

	return ShaderID;

}


GLuint loadShaders(const char * vertex_file_path,const char * fragment_file_path){

	// Create and compile shaders
	GLuint VertexShaderID = createAndCompileShader(vertex_file_path, GL_VERTEX_SHADER);
	GLuint FragmentShaderID = createAndCompileShader(fragment_file_path, GL_FRAGMENT_SHADER);

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	GLint Result = GL_FALSE;
	int InfoLogLength;

	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

