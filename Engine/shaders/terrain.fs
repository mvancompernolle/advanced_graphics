#version 430

struct AmbientLight {
    vec3 color;
    float intensity;
};

struct DirectionalLight {
    AmbientLight ambient;
    vec3 color;
    vec3 direction;
    float intensity;
};

struct Attenuation {
    float constant;
    float linear;
    float ex;
};

struct PointLight {
    AmbientLight ambient;
    vec3 pos;
};

uniform sampler2D gSampler;
uniform vec3 lightDir;

in vec2 fs_tex;
in vec3 fs_normal;

out vec4 color;

void main(void){

	// temp initialization

	// create a base ambient light for the scene
    AmbientLight light;
    light.color = vec3(1,1,1);
    light.intensity = .1;

    // create a direction light and give it the ambient light
   	DirectionalLight d_light;
    d_light.ambient = light;
    d_light.color = vec3(1,1,1);
    d_light.intensity = 0.75;
    d_light.direction = lightDir;

    // calulate the total light afftecting the pixel
    float diffIntense = max(0.0, dot(fs_normal, -d_light.direction)) * d_light.intensity;

    vec4 totalLight = vec4(d_light.ambient.color, 1.0) * (d_light.ambient.intensity + diffIntense);

	color = texture(gSampler, fs_tex.xy) * totalLight;
}