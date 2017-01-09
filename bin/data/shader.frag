#version 330

out vec4 outputColor;

in vec2 texCoordVarying;

uniform sampler2D koira;
uniform sampler2D kissa;

void main()
{

	if (texCoordVarying.x > 0.5)
		outputColor = texture(koira, texCoordVarying);
	else
		outputColor = texture(kissa, texCoordVarying);
	
	

}
