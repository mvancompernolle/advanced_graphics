#version 430

uniform sampler2D gColorMap;

in vec2 fs_tex;
in vec3 fs_normal;
in vec3 fs_pos;

layout (location = 0) out vec3 pos;
layout (location = 1) out vec3 diff;
layout (location = 2) out vec3 normal;
layout (location = 3) out vec3 texture;

void main(void){

    pos = fs_pos;
    diff = texture2D(gColorMap, fs_tex).xyz;
    normal = normalize(fs_normal);
    texture = vec3(fs_tex, 0.0);
}