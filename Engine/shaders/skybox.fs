#version 430

in vec3 fs_tex;

out vec4 fragColor;

uniform samplerCube textureUnit;
uniform vec3 lightDir;

void main(){

	float brightness = dot(vec3(0, -1, 0), lightDir);
	if(brightness < 0)
		brightness = 0;
	brightness += .1;

    fragColor = texture(textureUnit, fs_tex) * brightness;
}