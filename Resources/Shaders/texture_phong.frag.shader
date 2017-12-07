#version 430

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
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform vec3 lightColor;

uniform Material material;
uniform Light light;
uniform sampler2D textureSampler;

void main()
{

	vec3 positionToLight = normalize(light.position - vec3(outFragmentPosition));


	float diffuseIntensity = max(dot(positionToLight, outFragmentNormal), 0.0);
	vec3 diffuse = light.diffuse * material.diffuse * diffuseIntensity;
	vec3 ambient = light.ambient * material.ambient;

	vec3 specular = vec3(0.0);
	float specularIntensity = 0.0;
	if (diffuseIntensity > 0.0)
	{
		vec3 positionToView = normalize(-outFragmentPosition.xyz);
		vec3 reflectLightVector = reflect(-positionToLight, outFragmentNormal);
		specularIntensity = max(dot(reflectLightVector, positionToView), 0.0);
		specularIntensity = pow(specularIntensity, 32.0);
		specular = lightColor * material.specular * specularIntensity;
	}

	vec4 texColor = texture(textureSampler, outUVData);

	//outFragmentColor = texColor * vec4((ambient + diffuse), 1.0) + vec4(specular, 1.0);
	outFragmentColor =texColor;
}