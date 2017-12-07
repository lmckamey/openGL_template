# version 430


in vec3 tangentPosition;
in vec3 tangentLightPosition;
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

uniform Material material;
uniform Light light;

layout(binding = 0) uniform sampler2D textureSampler;
layout(binding = 1) uniform sampler2D normalMap;

void phong(vec3 position, vec3 normal, out vec3 ambientDiffuse, out vec3 specular)
{

	vec3 positionToLight = normalize(tangentLightPosition - position);

	float diffuseIntensity = max(dot(positionToLight, normal), 0.0);
	vec3 diffuse = light.diffuse * material.diffuse * diffuseIntensity;
	vec3 ambient = light.ambient * material.ambient;


	ambientDiffuse = ambient + diffuse;
	// specular calculations that were in main

	float specularIntensity = 0.0;
	if (diffuseIntensity > 0.0)
	{
		vec3 positionToView = normalize(-outFragmentPosition.xyz);
		vec3 reflectLightVector = reflect(-positionToLight, normal);
		specularIntensity = max(dot(reflectLightVector, positionToView), 0.0);
		specularIntensity = pow(specularIntensity, material.shininess);
	}
	specular = material.specular * specularIntensity;
}


void main()
{
	vec3 ambientDiffuse;
	vec3 specular;

	vec3 normal = texture(normalMap, outUVData).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	phong(tangentPosition, normal, ambientDiffuse, specular);

	vec4 textureColor = texture(textureSampler, outUVData);
	outFragmentColor = textureColor * vec4(ambientDiffuse, 1.0) + vec4(specular, 1.0);
	
}
