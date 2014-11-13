#version 430

struct BaseLight {
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
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

uniform sampler2D posMap;
uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;
uniform SpotLight spotLight;
uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform vec3 lightDir;
uniform vec2 screenSize;

out vec4 fragColor;

vec4 calcLightInternal(BaseLight l, vec3 lightDirection, vec3 pos, vec3 normal, vec2 specular){  

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

vec4 calcPointLight(PointLight p, vec3 pos, vec3 normal, vec2 specular){

    vec3 lightDirection = pos - p.pos;
    float dist = length(lightDirection);
    lightDirection = normalize(lightDirection);

    vec4 color = calcLightInternal(p.base, lightDirection, pos, normal, specular);
    float atten = p.atten.constant + p.atten.linear * dist + p.atten.ex * dist * dist;
    return color / atten;
}

vec4 calcSpotLight(SpotLight l, vec3 pos, vec3 normal, vec2 specular){

    vec3 lightToPixel = normalize(pos - l.base.pos);                             
    float spotFactor = dot(lightToPixel, l.direction);                                      
                                                                                            
    if (spotFactor > l.cutoff) {     

        vec4 color = calcPointLight(l.base, pos, normal, specular);                                        
        return color * (1.0 - (1.0 - spotFactor) * 1.0/(1.0 - l.cutoff));              
    }                                                                                       
    else {                                                                                  
        return vec4(0,0,0,0);                                                               
    }                                                                                       
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

	fragColor = vec4(color, 1.0) * calcSpotLight(spotLight, pos, normal, specular);
}