#version 430 core

out vec4 fColor;

uniform vec3 cameraPos;

in vec3 FragPos;
in vec3 Normal;
in vec3 FragColor;

void main()
{

	vec3 lightPos = vec3(2.0f, 1.0f, 2.0f);
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, Normal);
	vec3 viewDir = normalize(cameraPos - FragPos);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 0.5f);

	vec3 ambient = FragColor * 0.2;
	vec3 diffuse = FragColor * max(dot(Normal, normalize(lightPos - FragPos)), 0.0f);
	//vec3 specular = FragColor * spec;

	fColor = vec4(ambient + diffuse, 1.0f);

	float a = 0.2;
	float diff = max(dot(Normal, normalize(lightPos - FragPos)), 0.0f);
	fColor = (diff + a) * vec4(FragColor, 0.7f);	
	


}