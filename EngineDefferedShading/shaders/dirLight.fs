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

uniform sampler2D gPositionMap;
uniform sampler2D gColorMap;
uniform sampler2D gNormalMap;
uniform vec3 lightDir;
uniform vec2 screenSize;

out vec4 fragColor;

// lights
BaseLight light;
DirectionalLight d_light;

vec4 calcLightInternal(BaseLight l, vec3 lightDirection, vec3 pos, vec3 normal)                   
{                                                                                           
    vec4 ambientColor = vec4(l.color, 1.0f) * l.ambientIntensity;                   
    float diffuseFactor = dot(normal, -lightDirection);                                     
                                                                                            
    vec4 diffuseColor  = vec4(0, 0, 0, 0);                                                  
    vec4 specularColor = vec4(0, 0, 0, 0);                                                  
                                                                                            
    if (diffuseFactor > 0) {                                                                
        diffuseColor = vec4(l.color, 1.0f) * l.diffuseIntensity * diffuseFactor;    
                         

        specularColor = vec4((pos.xyz), 1.0).xyzw * .00001;                                                                   
        /*vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos0);                             
        vec3 LightReflect = normalize(reflect(LightDirection, Normal));                     
        float SpecularFactor = dot(VertexToEye, LightReflect);                              
        SpecularFactor = pow(SpecularFactor, gSpecularPower);                               
        if (SpecularFactor > 0) {                                                           
            SpecularColor = vec4(Light.Color, 1.0f) *                                       
                            gMatSpecularIntensity * SpecularFactor;                         
        }*/                                                                                  
    }                                                                                       
                                                                                            
    return (ambientColor + diffuseColor + specularColor);                                   
}  

vec4 calcDirectionLight(vec3 pos, vec3 normal){

    return calcLightInternal(d_light.base, d_light.direction, pos, normal);
} 

vec2 calcTexCoord(){

    return gl_FragCoord.xy / screenSize;
}

void main(void){

    // hard code directional light for now
	// create a base ambient light for the scene
    light.color = vec3(1.0, 1.0, 1.0);
    light.ambientIntensity = 0;
    light.diffuseIntensity = 0.9;

    // create a direction light
    d_light.base = light;
    d_light.direction = lightDir;

    vec2 texCoord = calcTexCoord();
    vec3 pos = texture(gPositionMap, texCoord).xyz;
    vec3 color = texture(gColorMap, texCoord).xyz;
    vec3 normal = texture(gNormalMap, texCoord).xyz;

	//fragColor = vec4(color, 1.0) * calcDirectionLight(pos, normal);
    fragColor = vec4(texCoord, 0.0, 1.0);
}