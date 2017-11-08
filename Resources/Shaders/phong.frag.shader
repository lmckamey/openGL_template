# version 430

in vec4 outFragmentPosition;
in vec3 outFragmentNormal;

out vec4 outFragmentColor;


uniform vec3 lightPosition;
uniform vec3 lightColor;

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

uniform Material material;
uniform Light light;

void main()
{

	vec3 positionToLight = normalize(lightPosition - vec3(outFragmentPosition));


	float diffuseIntensity = max(dot(positionToLight, outFragmentNormal), 0.0);
	vec3 diffuse = lightColor * material.diffuse * diffuseIntensity;
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

	outFragmentColor = vec4(ambient + diffuse + specular,1.0);
}