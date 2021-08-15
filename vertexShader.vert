#version 400 core

layout( location = 0 ) in vec3 vPosition;
layout( location = 1 ) in vec3 vColor;
layout( location = 2 ) in vec3 vNormal;

uniform mat4 modelViewProjection;

out vec4 vertexColor;

void main()
{
    vertexColor = vec4(vColor, 0.5);
    gl_Position = modelViewProjection * vec4(vPosition, 1.0);
}
