#version 430 core

out vec4 fColor;

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

uniform float u_time;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

in vec3 Normal;
in vec3 FragPos;

void main()
{
	//Ambient
	vec3 ambient = light.ambient * material.ambient;

	//Diffuse
	vec3 Norm = normalize(Normal);
	vec3 LightDir = normalize(light.position - FragPos);
	float diff = max(dot(Norm, LightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	//Specular
	vec3 viewDir = normalize(cameraPos - FragPos);
	vec3 reflectDir = reflect(-LightDir, Norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); // 32 - shininess value of an object (more - smaller cirlce)
	vec3 specular = light.specular * (spec * material.specular);


	vec3 result = (ambient + diffuse + specular);
	fColor = vec4(result, 1.0);
	//fColor = vec4(abs(cos(u_time)), y, x, 1.0);
}
