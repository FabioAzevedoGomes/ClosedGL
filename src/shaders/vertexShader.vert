#version 450 core

layout( location = 0 ) in vec3 vPosition;
layout( location = 1 ) in vec3 vColor;
layout( location = 2 ) in vec3 vNormal;

uniform mat4 modelViewProjection;
uniform vec3 uniformColor;

out vec4 vertexColor;

void main()
{
    vertexColor = vec4(uniformColor, 1.0);
    gl_Position = modelViewProjection * vec4(vPosition, 1.0);
}
