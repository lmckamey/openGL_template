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
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
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

void phong(vec3 position, vec3 normal, out vec3 ambientDiffuse, out vec3 specular)
{

	vec3 positionToLight = vec3(normalize(light.position - outFragmentPosition));
	vec3 ambient = light.ambient * material.ambient;

	float diffuseIntensity = max(dot(positionToLight, outFragmentNormal), 0.0);
	vec3 diffuse = (light.diffuse * material.diffuse * diffuseIntensity);

	ambientDiffuse = ambient + diffuse;
	// specular calculations that were in main

	float specularIntensity = 0.0;
	if (diffuseIntensity > 0.0)
	{
		vec3 positionToView = normalize(-outFragmentPosition.xyz);
		vec3 reflectLightVector = reflect(-positionToLight, outFragmentNormal);
		specularIntensity = max(dot(reflectLightVector, positionToView), 0.0);
		specularIntensity = pow(specularIntensity, material.shininess);
	}
		specular = (light.specular * material.specular * specularIntensity);

}

void main()
{
	vec3 ambientDiffuse;
	vec3 specular;
	phong(vec3(outFragmentPosition), outFragmentNormal, ambientDiffuse, specular);

	outFragmentColor = vec4(ambientDiffuse + specular, 1.0);
}