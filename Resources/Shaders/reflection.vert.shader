#version 430

subroutine vec3 reflectionModelType();

subroutine uniform reflectionModelType reflectionModel;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;

out vec3 reflectionDir;

uniform mat4 mxMVP;
uniform mat4 mxModel;
uniform vec3 cameraWorldPosition;


subroutine(reflectionModelType)
	vec3 skybox()
{
	return vertexPosition;
}

subroutine(reflectionModelType)
	vec3 reflection()
{
	vec3 worldPosition = vec3(mxModel * vec4(vertexPosition, 1.0));
	vec3 worldNormal = vec3(mxModel * vec4(vertexNormal, 0.0));
	vec3 worldView = normalize(cameraWorldPosition - worldPosition);

	return reflect(-worldView, worldNormal);
}

void main()
{
	reflectionDir = reflectionModel();
	gl_Position = mxMVP * vec4(vertexPosition, 1.0);
}
