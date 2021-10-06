#version 450 core

in vec4 vertexColor;
in vec4 normal;
in vec4 position;
in vec2 textureCoordinate;

uniform sampler2D canvasSampler;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

uniform vec3 uniformDiffuseColor;
uniform float uniformDiffuseIntensity;
uniform vec3 uniformAmbientColor;
uniform float uniformAmbientIntensity;
uniform vec3 uniformSpecularColor;
uniform float uniformSpecularIntensity;
uniform float uniformShineCoefficient;
uniform int openGLtextureEnabled;

out vec4 fColor;

subroutine vec4 FragmentLightingFunction();

subroutine (FragmentLightingFunction) vec4 OpenGL_PassThroughShader() {
    return vertexColor;
}

subroutine (FragmentLightingFunction) vec4 OpenGL_PhongShader() {
    vec4 cameraPosition=inverse(view)[3];
    
    // Light is assumed to be at the camera
    vec4 l=normalize(cameraPosition-(model*position));
    vec4 n=normalize(inverse(transpose(model))*normal);
    
    // Calculate lambert's diffuse term as Kd*I*dot(normal, light)
    vec3 diffuseTerm = uniformDiffuseColor * uniformDiffuseIntensity * max(0.0,dot(n,l));
    vec3 ambientTerm = uniformAmbientColor * vec3(uniformAmbientIntensity);

    // Calculate blinn-phong specular term
    vec4 h = normalize(2 * l);
    vec4 specularTerm = vec4(uniformSpecularColor, 1.0) * vec4(vec3(uniformSpecularIntensity),1.0) * pow(max(0, dot(n, h)), uniformShineCoefficient); 

    // Final color
    return vec4(diffuseTerm + ambientTerm, 0.0) + specularTerm;
}

subroutine (FragmentLightingFunction) vec4 Close2GL_TextureShader() {
    return texture(canvasSampler, textureCoordinate);
}

subroutine uniform FragmentLightingFunction fragmentLightingShader;

void main()
{
    vec4 color = fragmentLightingShader();

    if (openGLtextureEnabled != 0) {
        color = (0.8 * texture(canvasSampler, textureCoordinate)) + (0.2 * color);
    }

    fColor = color;
}
