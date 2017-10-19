#version 410

layout (location=0) in vec3 vertexPosition;
layout (location=1) in vec3 vertexColor;

out vec3 outVertexColor;

uniform mat4 modelMatrix;

void main()
{
    outVertexColor = vertexColor;
    gl_Position = modelMatrix * vec4(vertexPosition, 1.0);
}