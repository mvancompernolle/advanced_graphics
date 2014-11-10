#version 430

uniform sampler2D textureSampler;

in vec2 fs_tex;

out vec4 color;

void main(void){
	color = texture(textureSampler, fs_tex);

    if (color.r > 0.1) {           
        discard;                                                                    
    }   
}