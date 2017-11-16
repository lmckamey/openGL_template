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
uniform Light lights[5];

void phong(int lightIndex, vec3 position, vec3 normal, out vec3 diffuse, out vec3 specular) 
{
	// ambient/diffuse lighting calculations that were in main
	vec3 positionToLight = vec3(normalize(lights[lightIndex].position - outFragmentPosition));

	float diffuseIntensity = max(dot(positionToLight, outFragmentNormal), 0.0);
	vec3 tempDiffuse = lights[lightIndex].diffuse * material.diffuse * diffuseIntensity;


	diffuse = tempDiffuse;
	// specular calculations that were in main

	float specularIntensity = 0.0;
	if (diffuseIntensity > 0.0)
	{
		vec3 positionToView = normalize(-outFragmentPosition.xyz);
		vec3 reflectLightVector = reflect(-positionToLight, outFragmentNormal);
		specularIntensity = max(dot(reflectLightVector, positionToView), 0.0);
		specularIntensity = pow(specularIntensity, 32.0);
	}
	specular = lights[lightIndex].specular * material.specular * specularIntensity;

}

void main()
{
	vec3 color = material.ambient;
	for (int i = 0; i < 5; i++)
	{
		vec3 diffuse;
		vec3 specular;
		phong(i, vec3(outFragmentPosition), outFragmentNormal, diffuse, specular);
		color += (diffuse + specular);
	}

	outFragmentColor = vec4(color, 1.0);
}
