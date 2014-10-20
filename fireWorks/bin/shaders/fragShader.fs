#version 330

in float colorPos;
in vec3 heightColor;
uniform sampler1D gSampler;

void main(void){
   gl_FragColor = mix(texture(gSampler, colorPos), vec4(heightColor, 1), 0);
}
