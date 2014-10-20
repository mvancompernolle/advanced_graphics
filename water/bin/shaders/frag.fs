#version 330

in vec3 out_normal;
in vec3 world_pos;
out vec4 color;
uniform vec3 cameraPos;
uniform float specularStrength;
uniform float specularIntensity;

struct BaseLight{
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight{
	BaseLight base;
	vec3 direction;
};

struct Attenuation{
	float constant;
	float linear;
	float exp;
};

struct PointLight{
	BaseLight base;
	vec3 pos;
	Attenuation atten;
};

uniform BaseLight ambientLight;
uniform DirectionalLight directionalLight;
uniform PointLight pointLight;


vec4 calcLightBase(BaseLight light, vec3 lightDir, vec3 normal){
	vec4 ambColor = vec4(light.color, 1.0f) * light.ambientIntensity;

	float difFactor = dot(normal, lightDir);
	
	vec4 difColor = vec4(0.0,0.0,0.0,0.0);
	vec4 specularColor = vec4(0.0,0.0,0.0,0.0);
	
	if(difFactor > 0.0f){
		difColor = vec4(light.color, 1.0f) * light.diffuseIntensity * difFactor;
		vec3 vertToCamPos = normalize(cameraPos - world_pos);
		vec3 lightReflect = normalize(reflect(lightDir, normal));
		float specularFactor = dot(vertToCamPos, lightReflect);
		specularFactor = pow(specularFactor, specularStrength);
		if(specularFactor > 0.0f){
			specularColor = vec4(light.color, 1.0f) * specularIntensity * specularFactor;
		}
	}
	return (ambColor + difColor + specularColor);
}

vec4 calcDirectionalLight(vec3 normal){
	return calcLightBase(directionalLight.base, -directionalLight.direction, normal);
}

vec4 calcPointLight(PointLight light, vec3 normal){
	vec3 lightDir = world_pos - light.pos;
	float dist = length(lightDir);
	lightDir = normalize(lightDir);
	
	vec4 col = calcLightBase(light.base, -lightDir, normal);
	float attenuation = light.atten.constant + light.atten.linear * dist + light.atten.exp * dist * dist;

	return col/attenuation;
}

void main(void){
	vec3 normal = normalize(out_normal);
	vec4 totalLight = vec4(0.0, 0.0, 0.0, 0.0);

	// get light from directional light
	totalLight += calcDirectionalLight(normal);

	// get light from ambient light
	totalLight += vec4(ambientLight.color, 1.0f) * ambientLight.ambientIntensity;

	// get light from point light
	totalLight += calcPointLight(pointLight, normal);
	
	color = vec4(0, .1, .5, 1.0) * totalLight;

}