#version 430

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 UVdata;

out vec4 outFragmentPosition;
out vec3 outFragmentNormal;
out vec2 outUVData;


uniform mat4 mxModelView;
uniform mat4 mxMVP;
uniform mat3 mxNormal;

void main()
{
	outFragmentPosition = mxModelView * vec4(vertexPosition,1.0);
	outFragmentNormal = mxNormal * vertexNormal;

	gl_Position = mxMVP * vec4(vertexPosition, 1.0);
}