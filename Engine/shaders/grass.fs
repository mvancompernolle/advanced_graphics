#version 430

in vec3 fs_color;
in vec3 fs_normal;
in vec4 fs_pos;


layout (location = 0) out vec3 pos;
layout (location = 1) out vec3 diff;
layout (location = 2) out vec3 normal;
layout (location = 3) out vec2 specular;

void main(void){
    pos = fs_pos.xyz;
    diff = fs_color;
    normal = normalize(fs_normal);
    specular = vec2(0, 0);
}
