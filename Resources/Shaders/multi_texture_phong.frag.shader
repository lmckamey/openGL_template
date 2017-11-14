#version 430

in vec4 outFragmentPosition;
in vec3 outFragmentNormal;
in vec2 outUVData;

out vec4 outFragmentColor;

uniform vec3 ambientMaterial;
uniform vec3 diffuseMaterial;
uniform vec3 specularMaterial;

uniform vec3 lightPosition;
uniform vec3 lightColor;

uniform sampler2D textureSampler;
uniform sampler2D textureSampler2;

void main()
{

	vec3 positionToLight = normalize(lightPosition - vec3(outFragmentPosition));


	float diffuseIntensity = max(dot(positionToLight, outFragmentNormal), 0.0);
	//vec3 diffuse = lightColor * diffuseMaterial * diffuseIntensity;
	vec3 diffuse = lightColor * diffuseIntensity;
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

	vec4 texColor1 = texture(textureSampler, outUVData);
	vec4 texColor2	  = texture(textureSampler2, outUVData);

	vec4 texColor = mix(texColor1, texColor2, 0.5);


	outFragmentColor = texColor *vec4((ambient + diffuse), 1.0) + vec4(specular, 1.0);
	//outFragmentColor = texColor + vec4(specular, 1.0);
	outFragmentColor = texColor1;
}