#version 430

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;

out vec3 outVertexColor;

uniform vec3 ambientMaterial;
uniform vec3 diffuseMaterial;
uniform vec3 specularMaterial; 

uniform mat4 mxModelView;
uniform mat4 mxMVP;
uniform mat3 mxNormal;

uniform vec3 lightPosition;
uniform vec3 lightColor;

void main()
{
	
	vec3 tNormal = mxNormal * vertexNormal;  
	vec4 mvPosition = (mxModelView)* vec4(vertexPosition, 1.0);
	vec3 positionToLight = normalize(vec3(lightPosition - vec3(mvPosition)));
	
	
	float diffuseIntensity = max(dot(positionToLight, tNormal), 0.0);
	vec3 diffuse = lightColor * diffuseMaterial * diffuseIntensity;
	vec3 ambient = ambientMaterial;

	vec3 specular = vec3(0.0);
	float specularIntensity = 0.0;
	if (diffuseIntensity > 0.0)
	{
		vec3 positionToView = normalize(-mvPosition.xyz);
		vec3 reflectLightVector = reflect(-positionToLight, tNormal);
		specularIntensity = max(dot(reflectLightVector, positionToView), 0.0);
		specularIntensity = pow(specularIntensity, 1.0);
		specular = lightColor * specularMaterial * specularIntensity;
	}

	outVertexColor = ambient + diffuse + specularIntensity;
	gl_Position = mxMVP * vec4(vertexPosition, 1.0);
}