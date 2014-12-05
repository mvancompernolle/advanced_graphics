#version 430

uniform sampler2D textureUnit;

struct BaseLight {
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight {
    BaseLight base;
    vec3 direction;
};

in vec3 fs_pos;
in vec3 fs_normal;
in vec2 fs_texture;
out vec4 glColor;

uniform DirectionalLight dirLight;
uniform vec3 cameraPos;

vec4 calcLightInternal(BaseLight l, vec3 lightDirection, vec3 pos, vec3 normal, vec2 specular)                   {                                                                                           
    vec4 ambientColor = vec4(l.color, 1.0f) * l.ambientIntensity;                   
    float diffuseFactor = dot(vec3(0, 1, 0), -lightDirection);                                     
                                                                                            
    vec4 diffuseColor  = vec4(0, 0, 0, 0);                                                  
    vec4 specularColor = vec4(0, 0, 0, 0);                                                  
                                                                                            
    if (diffuseFactor > 0) {                                                                
        diffuseColor = vec4(l.color, 1.0f) * l.diffuseIntensity * diffuseFactor;    
                                                                                          
        vec3 vertexToEye = normalize(cameraPos - pos);                             
        vec3 lightReflect = normalize(reflect(lightDirection, normal));                     
        float specularFactor = dot(vertexToEye, lightReflect);                              
        specularFactor = pow(specularFactor, specular.x);                               
        if (specularFactor > 0) {                                                           
            specularColor = vec4(l.color, 1.0f) *                                       
                            specular.y * specularFactor;                         
        }                                                                                
    }                                                                                       
                                                                                            
    return (ambientColor + diffuseColor + specularColor);                                   
}  

vec4 calcDirectionLight(vec3 pos, vec3 normal, vec2 specular){

    return calcLightInternal(dirLight.base, dirLight.direction, pos, normal, specular);
} 

void main() {
    glColor = texture(textureUnit, fs_texture.xy) * calcDirectionLight(fs_pos, fs_normal, vec2(32.0, 1.0));
}
