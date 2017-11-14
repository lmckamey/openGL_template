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
	vec3 position;
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
		specular = light.diffuse * material.specular * specularIntensity;
	}

	float distance = abs(outFragmentPosition.z);
	float fogIntensity = distance - fog.distanceMin / fog.distanceMax - fog.distanceMin;
	fogIntensity = clamp(fogIntensity, 0.0, 1.0);

	vec4 mixColor = mix(vec4((ambient + diffuse),1.0) + vec4(specular, 1.0), vec4(fog.color, 1.0), fogIntensity);

	vec4 texColor = texture(textureSampler, outUVData);

	//outFragmentColor = vec4(mixColor,1.0) * vec4((ambient + diffuse), 1.0) + vec4(specular, 1.0);
	outFragmentColor = mixColor;
}