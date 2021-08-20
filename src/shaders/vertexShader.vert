#version 450 core

layout( location = 0 ) in vec3 vPosition;
layout( location = 1 ) in vec3 vColor;
layout( location = 2 ) in vec3 vNormal;

uniform vec3 uniformDiffuseColor;

uniform int orientation;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec4 vertexColor;

void main()
{    
    vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 cameraPosition = inverse(view) * origin;

    // Light is assumed to be at the camera
    vec4 l = normalize(cameraPosition - (model * vec4(vPosition, 1.0)));

    // Flip normal z axis based on current vertex orientation
    vec4 n = normalize(inverse(transpose(model)) * vec4(vNormal.x, vNormal.y, orientation * vNormal.z, 0.0));

    // Calculate lambert's diffuse term as Kd*I*dot(normal, light)
    vec3 lambert_diffuse_term = uniformDiffuseColor * vec3(1.0,1.0,1.0) * max(0,dot(n,l));

    // Cor final do fragmento
    vertexColor = vec4(lambert_diffuse_term, 1.0);

    gl_Position =  projection * view * model * vec4(vPosition, 1.0);
}
