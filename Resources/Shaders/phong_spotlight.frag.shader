# version 430

in vec4 outFragmentPosition;
in vec3 outFragmentNormal;
in vec2 outUVData;

out vec4 outFragmentColor;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec4 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float exponent;
	float cutoff;
};

struct Fog
{
	float distanceMin;
	float distanceMax;
	vec3 color;
};

uniform Fog fog;
uniform Material material;
uniform Light light;

uniform sampler2D textureSampler;

//void main()
//{
//
//
//
//	float diffuseIntensity = max(dot(positionToLight, outFragmentNormal), 0.0);
//	vec3 diffuse = light.diffuse * material.diffuse * diffuseIntensity;
//	vec3 ambient = light.ambient * material.ambient;
//
//	
//	float distance = abs(outFragmentPosition.z);
//	float fogIntensity = distance - fog.distanceMin / fog.distanceMax - fog.distanceMin;
//	fogIntensity = clamp(fogIntensity, 0.0, 1.0);
//
//	vec4 mixColor = mix(vec4((ambient + diffuse),1.0) + vec4(specular, 1.0), vec4(fog.color, 1.0), fogIntensity);
//
//	vec4 texColor = texture(textureSampler, outUVData);
//
//	//outFragmentColor = vec4(mixColor,1.0) * vec4((ambient + diffuse), 1.0) + vec4(specular, 1.0);
//	outFragmentColor = mixColor;
//}

void phong(vec3 position, vec3 normal, out vec3 ambientDiffuse, out vec3 specular)
{
	// ambient/diffuse lighting calculations that were in main
	vec3 positionToLight = vec3(normalize(light.position - outFragmentPosition));
	vec3 directionToLight;
	if (light.position.w == 0)
	{
		directionToLight = normalize(vec3(light.position));
	}
	else
	{
		directionToLight = normalize(vec3(light.position) - position);
	}

	vec3 spotDirection = normalize(light.direction);
	vec3 ambient = light.ambient * material.ambient;

	float angle = acos(dot(-directionToLight, spotDirection));
	float spotFactor = 1.0;
	if (angle < light.cutoff)
	{
		ambientDiffuse = ambient;
		specular = vec3(0.0f, 0.0f, 0.0f);
		spotFactor = pow(dot(-directionToLight, spotDirection), light.exponent);
	}


	float diffuseIntensity = max(dot(positionToLight, outFragmentNormal), 0.0);
	vec3 diffuse = (light.diffuse * material.diffuse * diffuseIntensity) * spotFactor; 

	ambientDiffuse = ambient + diffuse;
	// specular calculations that were in main

	float specularIntensity = 0.0;
	if (diffuseIntensity > 0.0)
	{
		vec3 positionToView = normalize(-outFragmentPosition.xyz);
		vec3 reflectLightVector = reflect(-positionToLight, outFragmentNormal);
		specularIntensity = max(dot(reflectLightVector, positionToView), 0.0);
		specularIntensity = pow(specularIntensity, 32.0);
	}
	specular = light.specular * material.specular * specularIntensity;

}

void main()
{
	vec3 ambientDiffuse;
	vec3 specular;
	phong(vec3(outFragmentPosition), outFragmentNormal, ambientDiffuse, specular);

	outFragmentColor = vec4(ambientDiffuse + specular, 1.0);
}

