#version 430

uniform sampler2D textureSampler;
uniform vec3 dropColor;
uniform int dropEnabled;

in vec2 fs_tex;

out vec4 color;

void main(void){
	color = texture(textureSampler, fs_tex);

    if (dropEnabled == 1 && abs(dropColor.r - color.r) < 0.7 && abs(dropColor.g - color.g) < 0.7
        && abs(dropColor.b - color.b) < 0.7) {           
        discard;                                                                    
    }   
}