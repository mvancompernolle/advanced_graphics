#version 430

struct BaseLight {
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight {
    BaseLight base;
    vec3 direction;
};

struct Attenuation {
    float constant;
    float linear;
    float ex;
};

struct PointLight {
    BaseLight base;
    vec3 pos;
    Attenuation atten;
};

struct SpotLight{
    PointLight base;
    vec3 direction;
    float cutoff;
};

uniform sampler2D gSampler;
uniform vec3 lightDir;
uniform vec3 spotLightPos;
uniform vec3 spotLightDir;
uniform vec3 cameraPos;
uniform float specularIntensity;
uniform float specularPower;
uniform int isSpecularOn;

in vec2 fs_tex;
in vec3 fs_normal;
in vec3 fs_pos;

out vec4 color;

// lights
BaseLight light;
DirectionalLight d_light;
PointLight p_light;
SpotLight s_light;

vec4 calcLightInternal(BaseLight l, vec3 lightDirection, vec3 normal)                   
{                                                                                           
    vec4 ambientColor = vec4(l.color, 1.0f) * l.ambientIntensity;                   
    float diffuseFactor = dot(normal, -lightDirection);                                     
                                                                                            
    vec4 diffuseColor  = vec4(0, 0, 0, 0);                                                  
    vec4 specularColor = vec4(0, 0, 0, 0);                                                  
                                                                                            
    if (diffuseFactor > 0) {                                                                
        diffuseColor = vec4(l.color, 1.0f) * l.diffuseIntensity * diffuseFactor;    
                                                                                            
        vec3 vertexToEye = normalize(cameraPos - fs_pos);                             
        vec3 lightReflect = normalize(reflect(lightDir, normal));                     
        float specularFactor = dot(vertexToEye, lightReflect);                              
        specularFactor = pow(specularFactor, specularPower);                               
        if (specularFactor > 0) {                                                           
            specularColor = vec4(l.color, 1.0f) *                                       
                            specularIntensity * specularFactor;                         
        }                                                                                  
    }                                                                                       
                                                                                            
    return (ambientColor + diffuseColor + (specularColor * isSpecularOn));                                   
}  

vec4 calcDirectionLight(vec3 normal){

    return calcLightInternal(d_light.base, d_light.direction, normal);
}

vec4 calcPointLight(PointLight p, vec3 normal){

    vec3 lightDirection = fs_pos - p.pos;
    float dist = length(lightDirection);
    lightDirection = normalize(lightDirection);

    vec4 color = calcLightInternal(p.base, lightDirection, normal);
    float atten = p.atten.constant + p.atten.linear * dist + p.atten.ex * dist * dist;
    return color / atten;
}

vec4 calcSpotLight(SpotLight l, vec3 normal){

    vec3 lightToPixel = normalize(fs_pos - l.base.pos);                             
    float spotFactor = dot(lightToPixel, l.direction);                                      
                                                                                            
    if (spotFactor > l.cutoff) {     
        //return calcPointLight(l.base, normal);                                                       
        vec4 color = calcPointLight(l.base, normal);                                        
        return color * (1.0 - (1.0 - spotFactor) * 1.0/(1.0 - l.cutoff));              
    }                                                                                       
    else {                                                                                  
        return vec4(0,0,0,0);                                                               
    }                                                                                       
}   

void main(void){

	// temp initialization

	// create a base ambient light for the scene
    light.color = vec3(1.0, 1.0, 1.0);
    light.ambientIntensity = 0.05;
    light.diffuseIntensity = 0.8;

    // create a direction light
    d_light.base = light;
    d_light.direction = lightDir;

    // create a point light
    Attenuation atten;
    atten.constant = 1;
    atten.linear = 0;
    atten.ex = 0.01;
    p_light.base = light;
    p_light.pos = vec3(30, 60.0, 0);
    p_light.atten = atten;

    // create a spot light
    Attenuation atten2;
    atten2.constant = 1;
    atten2.linear = .005;
    atten2.ex = 0.0;
    PointLight p;
    p.base = light;
    p.pos = spotLightPos;
    p.atten = atten2;
    s_light.base = p;
    s_light.direction = spotLightDir;
    s_light.cutoff = .99f;

    // calulate the total light afftecting the pixel
    vec4 totalLight = vec4(0,0,0,0);
    totalLight += calcDirectionLight(fs_normal);

    //totalLight += calcPointLight(p_light, fs_normal);

    totalLight += calcSpotLight(s_light, fs_normal);

	color = texture(gSampler, fs_tex.xy) * totalLight * (1 - (isSpecularOn * .2));
}