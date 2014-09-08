varying vec3 out_normal;
varying vec3 worldPos;
varying vec3 color;
varying float colorPos;
uniform sampler1D gSampler;
uniform vec3 cameraPos;
uniform float specularStrength;
uniform float specularIntensity;

// LIGHT REFERENCES: http://ogldev.atspace.co.uk/www/tutorial18/tutorial18.html (Tutorials 17 - 21)

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

struct SpotLight{
	PointLight base;
	vec3 direction;
	float radius;
};

uniform BaseLight ambientLight;
uniform DirectionalLight directionalLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;

vec4 calcLightBase(BaseLight light, vec3 lightDir, vec3 normal){
	vec4 ambColor = vec4(light.color, 1.0f) * light.ambientIntensity;

	float difFactor = dot(normal, lightDir);
	
	vec4 difColor = vec4(0.0,0.0,0.0,0.0);
	vec4 specularColor = vec4(0.0,0.0,0.0,0.0);
	
	if(difFactor > 0.0f){
		difColor = vec4(light.color, 1.0f) * light.diffuseIntensity * difFactor;
		vec3 vertToCamPos = normalize(cameraPos - worldPos);
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
	vec3 lightDir = worldPos - light.pos;
	float dist = length(lightDir);
	lightDir = normalize(lightDir);
	
	vec4 col = calcLightBase(light.base, -lightDir, normal);
	float attenuation = light.atten.constant + light.atten.linear * dist + light.atten.exp * dist * dist;

	return col/attenuation;
}

vec4 calcSpotLight(SpotLight l, vec3 normal){
	
	vec3 lightToPix = normalize(worldPos - l.base.pos);
	float spotFactor = dot(lightToPix, l.direction);
	
	if(spotFactor > l.radius){
		vec4 color = calcPointLight(l.base, normal);
		return color * (1.0 - (1.0 - spotFactor) * 1.0/(1.0 - l.radius));
	}	
	else{
		return vec4(0.0, 0.0, 0.0, 0.0);
	}
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

	// get light from spot light
	totalLight += calcSpotLight(spotLight, normal);
	
	gl_FragColor = texture1D(gSampler, colorPos) * totalLight;

}
