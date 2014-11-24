#version 430

in vec3 fs_tex;

out vec4 fragColor;

uniform samplerCube textureUnit;

void main(){

    fragColor = texture(textureUnit, fs_tex);
}