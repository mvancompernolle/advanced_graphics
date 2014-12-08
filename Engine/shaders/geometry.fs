#version 430

uniform sampler2D colorMap;
uniform float specularPower;
uniform float specularIntensity;

in vec2 fs_tex;
in vec3 fs_normal;
in vec3 fs_pos;

layout (location = 0) out vec3 pos;
layout (location = 1) out vec3 diff;
layout (location = 2) out vec3 normal;
layout (location = 3) out vec2 specular;

void main(void){
    pos = fs_pos;
    diff = texture2D(colorMap, fs_tex).xyz;
    normal = normalize(fs_normal);
    specular = vec2(specularPower, specularIntensity);
}