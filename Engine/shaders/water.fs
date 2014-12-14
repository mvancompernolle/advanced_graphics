#version 430

uniform sampler2D textureUnit;

in vec3 fs_pos;
in vec3 fs_normal;
in vec2 fs_texture;

layout (location = 0) out vec3 pos;
layout (location = 1) out vec3 diff;
layout (location = 2) out vec3 normal;
layout (location = 3) out vec2 specular;

void main() {
    pos = fs_pos;
    diff = texture(textureUnit, fs_texture).xyz;
    normal = normalize(fs_normal);
    specular = vec2(100, .5);
}
