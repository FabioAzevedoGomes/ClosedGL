#version 450 core

layout( location = 0 ) in vec3 vPosition;
layout( location = 1 ) in vec3 vColor;
layout( location = 2 ) in vec3 vNormal;

// Mvp
uniform mat4 modelViewProjection;
uniform mat4 viewMatrix;

// Shading & Coloring
uniform vec4 cameraPosition;
uniform vec4 lightPosition;
uniform vec3 uniformColor;

// Orientation
uniform int orientation;

out vec3 vertexColor;
out vec3 vertexNormal;
out vec3 vertexPosition;
out vec3 lightPos;

void main()
{
    vertexPosition = vPosition;
    vertexColor = uniformColor;
    vertexNormal = vNormal;
    lightPos = vec3(lightPosition);
    gl_Position = modelViewProjection * vec4(vPosition, 1.0);
}
