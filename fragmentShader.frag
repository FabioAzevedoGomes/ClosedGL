#version 450 core

in vec3 vertexColor;

in vec3 vertexNormal;
in vec3 vertexPosition;
in vec3 lightPos;

out vec4 fColor;

void main()
{
    vec3 lightVector = normalize(lightPos-vertexPosition);
    float difference = max(dot(normalize(vertexNormal), lightVector), 0.1);
    vec4 diffuse = difference * vec4(1.0,1.0,1.0,1.0);
    fColor = diffuse * vec4(vertexColor, 1.0);
}
