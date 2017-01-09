#include "ofMain.h"

ofVec3f parsev3(std::string line);
ofVec2f parsev2(std::string line);

ofMatrix4x4 newLookAtMatrixGLMway(const ofVec3f& eye, const ofVec3f& center, const ofVec3f& up) {
	ofMatrix4x4 m;
	ofVec3f zaxis = (eye - center).getNormalized();
	ofVec3f xaxis = up.getCrossed(zaxis).getNormalized();
	ofVec3f yaxis = zaxis.getCrossed(xaxis);


	m._mat[0].set(xaxis.x, yaxis.x, zaxis.x, 0);
	m._mat[1].set(xaxis.y, yaxis.y, zaxis.y, 0);
	m._mat[2].set(xaxis.z, yaxis.z, zaxis.z, 0);
	m._mat[3].set(-eye.dot(xaxis), -eye.dot(yaxis), -eye.dot(zaxis), 1);
	return m;
}


ofMesh readObjFileSimplified(std::string file_path){
	ofMesh newMesh;
	std::ifstream objStream(file_path, std::ios::in);
	if(objStream.is_open()){
			std::string line = "";
			while(getline(objStream, line))
			{
				std::string temp=line.substr(0,2);

				if(!temp.compare("v ")){
					ofVec3f vertex = parsev3(line);
					newMesh.addVertex(vertex);
				}
				else if(!temp.compare("vt")){
					ofVec2f uv = parsev2(line);
					newMesh.addTexCoord(uv);
				}

			}
		objStream.close();

	}
	return newMesh;

}

ofVec3f parsev3(std::string line){
	ofVec3f point;
	unsigned int i=0;
	unsigned int k=0;
	bool flag=false;
	std::string temp;
	for (unsigned j=1; j< line.length(); j++){
		temp=line[j];
		if(!temp.compare(" ") || !temp.compare("\t"))
		{
			if(flag)
			{
				std::string temp2=line.substr(i+1,j-i);
				point[k]=atof(temp2.c_str());
				k++;
			}
			i=j;
			flag=false;
		}
		else{
			flag=true;
		}
	}
	if ((k<3) && flag){
		std::string temp2=line.substr(i+1,line.length()-1);
		point[k]=atof(temp2.c_str());
	}
	return point;

}

ofVec2f parsev2(std::string line){
	ofVec2f point;
	unsigned int i=0;
	unsigned int k=0;
	bool flag=false;
	std::string temp;
	for (unsigned j=2; j< line.length(); j++){
		temp=line[j];
		if(!temp.compare(" ") || !temp.compare("\t"))
		{
			if(flag)
			{
				std::string temp2=line.substr(i+1,j-i);
				point[k]=atof(temp2.c_str());
				k++;
			}
			i=j;
			flag=false;
		}
		else{
			flag=true;
		}
	}
	if ((k<2) && flag){
		std::string temp2=line.substr(i+1,line.length()-1);
		point[k]=atof(temp2.c_str());
	}
	return point;

}


