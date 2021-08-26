#version 450 core

layout(location=0)in vec3 openGLvPosition;
layout(location=1)in vec3 openGLvColor;
layout(location=2)in vec3 openGLvNormal;

layout(location=3)in vec2 close2GLvPosition;
layout(location=4)in vec3 close2GLvColor;

uniform vec3 uniformDiffuseColor;
uniform vec3 uniformAmbientColor;
uniform vec3 uniformSpecularColor;
uniform float uniformShineCoefficient;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec4 vertexColor;

subroutine vec4 LightingFunction();
subroutine void PositionFunction();

// Lighting options
subroutine (LightingFunction) vec4 OpenGL_FlatShader() {
    return vec4(uniformDiffuseColor,1.);
}

subroutine (LightingFunction) vec4 OpenGL_AD_GouraudShader() {
    vec4 cameraPosition=inverse(view)[3];
    
    // Light is assumed to be at the camera
    vec4 l=normalize(cameraPosition-(model*vec4(openGLvPosition,1.)));
    vec4 n=normalize(inverse(transpose(model))*vec4(openGLvNormal,0.));
    
    // Calculate lambert's diffuse term as Kd*I*dot(normal, light)
    vec3 diffuseTerm = uniformDiffuseColor * vec3(1.0, 1.0, 1.0) * max(0.2,dot(n,l)); // @TODO Intensity
    vec3 ambientTerm = uniformAmbientColor * vec3(0.1, 0.1, 0.1); // @TODO Intensity

    // Final color
    return vec4(diffuseTerm + ambientTerm, 1.0);
}

subroutine (LightingFunction) vec4 OpenGL_ADS_GouraudShader() {
    vec4 cameraPosition=inverse(view)[3];
    
    // Light is assumed to be at the camera
    vec4 l=normalize(cameraPosition-(model*vec4(openGLvPosition,1.)));
    vec4 n=normalize(inverse(transpose(model))*vec4(openGLvNormal,0.));
    
    // Calculate lambert's diffuse term as Kd*I*dot(normal, light)
    vec3 diffuseTerm = uniformDiffuseColor * vec3(1.0, 1.0, 1.0) * max(0.0,dot(n,l)); // @TODO Intensity
    vec3 ambientTerm = uniformAmbientColor * vec3(0.1, 0.1, 0.1); // @TODO Intensity

    // Calculate blinn-phong specular term as Ks*I*pow( max(0,dot(normal, intensity)), q)
    vec4 h = normalize(2 * l);
    vec4 specularTerm = vec4(uniformSpecularColor, 1.0) * vec4(1.0,1.0,1.0,1.0) * pow(max(0, dot(n, h)), uniformShineCoefficient); // @TODO Intensity

    // Final color
    return vec4(diffuseTerm + ambientTerm, 0.0) + specularTerm;
}

// Vertex position options
subroutine (PositionFunction) void OpenGL_VetexCoords() {
    gl_Position = projection * view * model * vec4(openGLvPosition, 1.0);
}

subroutine (PositionFunction) void Close2GL_VertexCoords() {
    gl_Position = vec4(close2GLvPosition, 1.0, 1.0);
}

subroutine uniform LightingFunction lightingShader;
subroutine uniform PositionFunction positionShader;

void main() {
    vertexColor = lightingShader();
    positionShader();
}
