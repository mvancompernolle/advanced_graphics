#version 430

in vec2 fs_tex;

uniform sampler2D gSampler;

out vec4 color;

void main(){

	float depth = texture(gSampler, fs_tex).x;
	color = vec4(depth);
}