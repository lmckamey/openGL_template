#version 430

in vec3 outVertexColor;

layout (location=0) out vec4 outFragmentColor;

void main()
{
    outFragmentColor = vec4(outVertexColor, 1.0);
}