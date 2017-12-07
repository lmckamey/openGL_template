#version 430

in vec3 reflectionDir;

layout(binding = 0) uniform samplerCube cubeMapTexture;
layout(location = 0) out vec4 outColorFragment;

void main()
{
	vec4 cubeMapColor = texture(cubeMapTexture, reflectionDir);
	outColorFragment = cubeMapColor;
}
