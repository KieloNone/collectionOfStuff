#version 330

out vec4 outputColor;

in vec2 texCoordVarying;
//in vec3 texCoordCube;
in float R;
in vec3 N;
in vec3 N2;
in vec3 V;


uniform sampler2D texSampler;
uniform samplerCube skybox;

void main()
{
	vec3 texCoordCube=2*V+N;
	vec4 texColor = texture(texSampler, texCoordVarying);
	vec4 refColor = texture(skybox, texCoordCube);
	vec3 L=vec3(1,1.0,0.0);
	float diffuse=max(0.0,dot(L,N2));
	
	
	outputColor=mix(refColor,diffuse*texColor,0.3);
	

}
