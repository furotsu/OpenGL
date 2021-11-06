#version 430 core

out vec4 fColor;

struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	float shininess;
};

struct DirLight
{
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

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

struct FlashLight
{
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float innerCutOff;
	float outerCutOff;

	float constantIntens;
	float linearIntens;
	float quadraticIntens;
};

#define NR_POINT_LIGHTS 4

uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform FlashLight flashLight;
uniform DirLight dirLight;

uniform Material material;

uniform vec3 cameraPos;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(FlashLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	
	//fColor = vec4(result, 1.0);
	//fColor = vec4(abs(cos(u_time)), y, x, 1.0);
	// properties
	//vec3 norm = normalize(Normal);
	//vec3 viewDir = normalize(cameraPos - FragPos);
	//vec3 result = vec3(0.0f);
	// phase 1: Directional lighting
	//vec3 result = CalcDirLight(dirLight, norm, viewDir);
	// phase 2: Point lights
	//for (int i = 0; i < NR_POINT_LIGHTS; i++)
		//result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	// phase 3: Spot light
	//result += CalcSpotLight(flashLight, norm, FragPos, viewDir);    

	fColor = vec4(1.0, 1.0, 1.0, 1.0);
}


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(normal, lightDir), 0.0f);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constantIntens + light.linearIntens * distance + light.quadraticIntens * (distance * distance));
	// combine results
	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
	ambient  *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(FlashLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constantIntens + light.linearIntens * distance + light.quadraticIntens * (distance * distance));

	vec3 LightDir = normalize(light.position - fragPos);

	float theta = dot(LightDir, normalize(-light.direction));
	float epsilon = light.innerCutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

	float diff = max(dot(normal, LightDir), 0.0f);
	
	vec3 reflectDir = reflect(-LightDir, normal);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	
	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
	ambient  *= attenuation;
	diffuse  *= attenuation * intensity;
	specular *= attenuation * intensity;
	
	return (ambient + diffuse + specular);
}


	/*
	float distance = length(flashLight.position - FragPos);
	float attenuation = 1.0f / (flashLight.constantIntens + flashLight.linearIntens * distance + flashLight.quadraticIntens * (distance * distance));

	vec3 LightDir = normalize(flashLight.position - FragPos);// if light is directional

	float theta = dot(LightDir, normalize(-flashLight.direction));
	float epsilon = flashLight.innerCutOff - flashLight.outerCutOff;
	float intensity = clamp((theta - flashLight.outerCutOff) / epsilon, 0.0f, 1.0f);

	//Ambient
	vec3 ambient = flashLight.ambient * vec3(texture(material.diffuse, TexCoords));// *vec3(texture(material.diffuse, TexCoords));

	//Diffuse
	vec3 Norm = normalize(Normal);
	//vec3 LightDir = normalize(flashLight.position - FragPos); // if light is coming from a point
	float diff = max(dot(Norm, LightDir), 0.0);
	vec3 diffuse = flashLight.diffuse * diff * vec3(texture(material.diffuse, TexCoords)) * attenuation * intensity;// *vec3(texture(material.diffuse, TexCoords));

	//Specular
	vec3 viewDir = normalize(cameraPos - FragPos);
	vec3 reflectDir = reflect(-LightDir, Norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); // 32 - shininess value of an object (more - smaller cirlce)
	vec3 specular = flashLight.specular * spec * vec3(texture(material.specular, TexCoords)) * attenuation * intensity;


	vec3 result = (ambient + diffuse + specular);
	fColor = vec4(result, 1.0f);*/
