#version 430

in vec3 positionFragment;
in vec3 normalFragment;
in vec2 texCoordFragment;
in vec4 shadowCoordFragment;

layout(location = 0) out vec4 outFragmentColor;

struct MaterialInfo
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform MaterialInfo material;

struct LightInfo
{
	vec4 position;
	vec3 diffuse;
	vec3 specular;
};

uniform LightInfo light;

layout(binding = 0) uniform sampler2D diffuseMap;
layout(binding = 1) uniform sampler2DShadow shadowMap;

float calculateShadow()
{
	float bias = 0.005;
	// shadow = 1.0 (not in shadow)
	// shadow = 0.0 (in shadow)
	float shadow = texture(shadowMap, vec3(shadowCoordFragment.xy, (shadowCoordFragment.z) / shadowCoordFragment.w) - bias);

	return shadow;
}

void phong(vec3 position, vec3 normal, out vec3 diffuse, out vec3 specular)
{
	// DIFFUSE
	vec3 lightDirection = normalize(vec3(light.position) - position);
	float diffuseIntensity = max(dot(lightDirection, normal), 0.0);
	diffuse = light.diffuse * material.diffuse * diffuseIntensity;

	// SPECULAR
	specular = vec3(0.0, 0.0, 0.0);
	if (diffuseIntensity > 0.0)
	{
		vec3 positionToViewDirection = normalize(-position.xyz);
		vec3 reflectLightDirection = reflect(-lightDirection, normal);
		float specularIntensity = max(dot(reflectLightDirection, positionToViewDirection), 0.0);
		specularIntensity = pow(specularIntensity, material.shininess);
		specular = light.specular * material.specular * specularIntensity;
	}
}

void main()
{
	vec3 ambient = material.ambient;
	vec3 diffuse;
	vec3 specular;
	phong(positionFragment, normalFragment, diffuse, specular);

	vec4 textureColor = texture(diffuseMap, texCoordFragment);
	float shadow = calculateShadow();
	outFragmentColor = textureColor * vec4(ambient + ((diffuse + specular) * shadow), 1.0);
}


