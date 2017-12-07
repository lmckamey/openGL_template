#version 430

in vec2 texCoordFragment;

layout(location = 0) out vec4 outFragmentColor;
layout(binding = 0) uniform sampler2D depthMap;

void main()
{
	float depthValue = texture(depthMap, texCoordFragment).x;
	outFragmentColor = vec4(vec3(depthValue), 1.0);
}
