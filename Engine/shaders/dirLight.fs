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

uniform sampler2D posMap;
uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;
uniform DirectionalLight dirLight;
uniform vec3 cameraPos;
uniform vec3 lightDir;
uniform vec2 screenSize;

out vec4 fragColor;

vec4 calcLightInternal(BaseLight l, vec3 lightDirection, vec3 pos, vec3 normal, vec2 specular)                   
{                                                                                           
    vec4 ambientColor = vec4(l.color, 1.0f) * l.ambientIntensity;                   
    float diffuseFactor = dot(normal, -lightDirection);                                     
                                                                                            
    vec4 diffuseColor  = vec4(0, 0, 0, 0);                                                  
    vec4 specularColor = vec4(0, 0, 0, 0);                                                  
                                                                                            
    if (diffuseFactor > 0) {                                                                
        diffuseColor = vec4(l.color, 1.0f) * l.diffuseIntensity * diffuseFactor;    
                                                                                          
        vec3 vertexToEye = normalize(cameraPos - pos);                             
        vec3 lightReflect = normalize(reflect(lightDir, normal));                     
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

vec2 calcTexCoord(){

    return gl_FragCoord.xy / screenSize;
}

void main(void){

    vec2 texCoord = calcTexCoord();
    vec3 pos = texture(posMap, texCoord).xyz;
    vec3 color = texture(colorMap, texCoord).xyz;
    vec3 normal = texture(normalMap, texCoord).xyz;
    vec2 specular = texture(specularMap, texCoord).xy;
    specular = vec2(10, 10);

	fragColor = vec4(color, 1.0) * calcDirectionLight(pos, normal, specular);
}