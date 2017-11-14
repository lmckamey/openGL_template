#version 430

in vec3 colorFragment;

layout (location=0) out vec4 outFragmentColor;

void main()
{
    outFragmentColor = vec4(colorFragment , 1.0);
}