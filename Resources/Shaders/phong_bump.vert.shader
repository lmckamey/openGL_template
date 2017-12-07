#version 430

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 UVdata;
layout(location = 3) in vec3 tangentVertex;

out vec3 tangentPosition;
out vec3 tangentLightPosition;
out vec4 outFragmentPosition;
out vec3 outFragmentNormal;
out vec2 outUVData;

uniform vec4 lightPosition;
uniform mat4 mxModelView;
uniform mat4 mxMVP;
uniform mat3 mxNormal;

void main()
{

	//tangent Calculations
	vec3 normal = normalize(mxNormal * vertexNormal);
	vec3 tangent = normalize(mxNormal * tangentVertex);

	vec3 bitangent = cross(normal, tangent);
	mat3 tangentSpace = transpose(mat3(tangent, bitangent, normal));

	tangentLightPosition = tangentSpace * lightPosition.xyz;

	vec3 position = vec3(mxModelView * vec4(vertexPosition, 1.0));
	tangentPosition = tangentSpace * position;

	outFragmentPosition = mxModelView * vec4(vertexPosition,1.0);
	outFragmentNormal = mxNormal * vertexNormal;
	outUVData = UVdata;

	gl_Position = mxMVP * vec4(vertexPosition, 1.0);
}