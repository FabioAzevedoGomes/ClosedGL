#version 450 core

layout(location=0)in vec3 openGLvPosition;
layout(location=1)in vec3 openGLvColor;
layout(location=2)in vec3 openGLvNormal;
layout(location=5)in vec2 openGLvTextureCoordinate;

layout(location=3)in vec4 close2GLvPosition;
layout(location=4)in vec2 close2GLvTextureCoordinate;

uniform vec3 uniformDiffuseColor;
uniform float uniformDiffuseIntensity;
uniform vec3 uniformAmbientColor;
uniform float uniformAmbientIntensity;
uniform vec3 uniformSpecularColor;
uniform float uniformSpecularIntensity;
uniform float uniformShineCoefficient;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec4 vertexColor;
out vec4 normal;
out vec4 position;
out vec2 textureCoordinate;

subroutine vec4 VertexLightingFunction();
subroutine void VertexPositionFunction();

// Lighting options
subroutine (VertexLightingFunction) vec4 OpenGL_FlatShader() {
    return vec4(uniformDiffuseColor,1.) * uniformDiffuseIntensity;
}

subroutine (VertexLightingFunction) vec4 OpenGL_AD_GouraudShader() {
    vec4 lightPos = inverse(view) * vec4(2.0,2.0,2.0,1.0);
    vec4 cameraPosition=inverse(view)[3];
    
    // Light is assumed to be at the camera
    vec4 l=normalize(lightPos-(model*vec4(openGLvPosition,1.)));
    vec4 n=normalize(inverse(transpose(model))*vec4(openGLvNormal,0.));
    
    // Calculate lambert's diffuse term as Kd*I*dot(normal, light)
    vec3 diffuseTerm = uniformDiffuseColor * uniformDiffuseIntensity * max(0.2,dot(n,l));
    vec3 ambientTerm = uniformAmbientColor * vec3(uniformAmbientIntensity);

    // Final color
    return vec4(diffuseTerm + ambientTerm, 1.0);
}

subroutine (VertexLightingFunction) vec4 OpenGL_ADS_GouraudShader() {
    vec4 lightPos = inverse(view) * vec4(2.0,2.0,2.0,1.0);
    vec4 cameraPosition=inverse(view)[3];
    
    vec4 v=normalize(cameraPosition - vec4(openGLvPosition,1.));
    vec4 l=normalize(lightPos-(model*vec4(openGLvPosition,1.)));
    vec4 n=normalize(inverse(transpose(model))*vec4(openGLvNormal,0.));
    
    // Calculate lambert's diffuse term as Kd*I*dot(normal, light)
    vec3 diffuseTerm = uniformDiffuseColor * uniformDiffuseIntensity * max(0.0,dot(n,l));
    vec3 ambientTerm = uniformAmbientColor * vec3(uniformAmbientIntensity);

    // Calculate blinn-phong specular term
    vec4 h = normalize(l + v);
    vec4 specularTerm = vec4(uniformSpecularColor, 1.0) * vec4(vec3(uniformSpecularIntensity),1.0) * pow(max(0, dot(n, h)), uniformShineCoefficient); 

    // Final color
    return vec4(diffuseTerm + ambientTerm, 0.0) + specularTerm;
}

subroutine (VertexLightingFunction) vec4 OpenGL_PhongShader() {
    position = vec4(openGLvPosition,1.0);
    normal = vec4(openGLvNormal,0.0);
    return vec4(uniformDiffuseColor,1.);
}

subroutine (VertexLightingFunction) vec4 Close2GL_TextureShader() {
    textureCoordinate = close2GLvTextureCoordinate;
    return vec4(0.0);
}

// Vertex position options
subroutine (VertexPositionFunction) void OpenGL_VetexCoords() {
    textureCoordinate = openGLvTextureCoordinate;
    gl_Position = projection * view * model * vec4(openGLvPosition, 1.0);
}

subroutine (VertexPositionFunction) void Close2GL_VertexCoords() {
    gl_Position = close2GLvPosition;
}

subroutine uniform VertexLightingFunction vertexLightingShader;
subroutine uniform VertexPositionFunction vertexPositionShader;

void main() {
    vertexColor = vertexLightingShader();
    vertexPositionShader();
}
