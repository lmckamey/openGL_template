#version 430

layout(location = 0) in vec3 vertexPosition;

uniform mat4 mxLVP;
uniform mat4 mxModel;

void main()
{
	gl_Position = mxLVP * mxModel * vec4(vertexPosition, 1.0);
}
