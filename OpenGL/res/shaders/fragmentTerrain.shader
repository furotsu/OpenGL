#version 430 core

out vec4 fColor;

uniform sampler2D texture_diffuse;
uniform vec3 cameraPos;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 vColor;

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	//vec3 specular;
};

//uniform DirLight dirLight;

//vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);


void main()
{
	/*
	vec3 lightPos = vec3(2.0f, 5.0f, 2.0f);
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, Normal);
	vec3 viewDir = normalize(cameraPos - FragPos);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 0.5f);

	vec3 ambient = FragColor * 0.2;
	vec3 diffuse = FragColor  * max(dot(Normal, normalize(lightPos - FragPos)), 0.0f);
	//vec3 specular = FragColor * spec;
	*/
	fColor = vec4(vColor + vec3(texture(texture_diffuse, TexCoords)), 1.0f);
}

