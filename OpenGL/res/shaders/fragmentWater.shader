#version 430 core

out vec4 fColor;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

uniform vec3 cameraPos;

in vec3 FragPos;
in vec3 Normal;
in vec3 FragColor;
in vec4 clipSpace;
in vec3 resPos;
in vec2 textureCoords;

struct PointLight
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constantIntens;
	float linearIntens;
	float quadraticIntens;
};

uniform PointLight pointLights[1];

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


void main()
{
	vec2 ndc = (clipSpace.xy / clipSpace.w) / 2.0f + 0.5f;

	vec2 reflectionTexCoords = vec2(ndc.x, -ndc.y)  + textureCoords;
	vec2 refractionTexCoords = vec2(ndc.x, -ndc.y) + textureCoords;


	vec3 lightPos = vec3(2.0f, 5.0f, 2.0f);
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, Normal);
	vec3 viewDir = normalize(cameraPos - FragPos);

	float a = 0.1;
	float diff = max(dot(Normal, normalize(lightPos - FragPos)), 0.0f);

	vec3 reflectionCol = texture(reflectionTexture, reflectionTexCoords).xyz;
	vec3 refractionCol = texture(refractionTexture, refractionTexCoords).xyz;

	vec3 combinedColor = CalcPointLight(pointLights[0], Normal, FragPos, viewDir) * 0.8f +  0.2f * reflectionCol;

	//fColor = (diff + a) * vec4(combinedColor, 1.0f);
	fColor = vec4(combinedColor, 0.8f);	
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0f); // shininess
	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constantIntens + light.linearIntens * distance + light.quadraticIntens * (distance * distance));
	// combine results
	vec3 ambient = light.ambient * FragColor;
	vec3 diffuse = light.diffuse * diff * FragColor;
	vec3 specular = light.specular * spec * FragColor;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}