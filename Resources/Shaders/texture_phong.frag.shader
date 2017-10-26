#version 430

in vec4 outFragmentPosition;
in vec3 outFragmentNormal;
in vec2 outUVdata;

out vec4 outFragmentColor;

uniform vec3 ambientMaterial;
uniform vec3 diffuseMaterial;
uniform vec3 specularMaterial;

uniform vec3 lightPosition;
uniform vec3 lightColor;

uniform 

void main()
{

	vec3 positionToLight = normalize(lightPosition - vec3(outFragmentPosition));


	float diffuseIntensity = max(dot(positionToLight, outFragmentNormal), 0.0);
	vec3 diffuse = lightColor * diffuseMaterial * diffuseIntensity;
	vec3 ambient = ambientMaterial;

	vec3 specular = vec3(0.0);
	float specularIntensity = 0.0;
	if (diffuseIntensity > 0.0)
	{
		vec3 positionToView = normalize(-outFragmentPosition.xyz);
		vec3 reflectLightVector = reflect(-positionToLight, outFragmentNormal);
		specularIntensity = max(dot(reflectLightVector, positionToView), 0.0);
		specularIntensity = pow(specularIntensity, 32.0);
		specular = lightColor * specularMaterial * specularIntensity;
	}

	outFragmentColor = vec4(ambient + diffuse + specular, 1.0);
}