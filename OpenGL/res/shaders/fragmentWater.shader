#version 430 core

out vec4 fColor;

uniform sampler2D reflectionTexture;

uniform vec3 cameraPos;

in vec3 FragPos;
in vec3 Normal;
in vec3 FragColor;
in vec4 clipSpace;
in vec3 resPos;
in vec2 textureCoords;


void main()
{
	vec2 ndc = (clipSpace.xy / clipSpace.w) / 2.0f + 0.5f;

	vec2 reflectionTexCoords = vec2(ndc.x, -ndc.y)  + textureCoords;


	vec3 lightPos = vec3(0.0f, 0.5f, 2.0f);
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, Normal);
	vec3 viewDir = normalize(cameraPos - FragPos);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 0.5f);

	vec3 ambient = FragColor * 0.2;
	vec3 diffuse = FragColor * max(dot(Normal, normalize(lightPos - FragPos)), 0.0f);
	//vec3 specular = FragColor * spec;

	float a = 0.1;
	float diff = max(dot(Normal, normalize(lightPos - FragPos)), 0.0f);

	vec3 combinedColor = FragColor * 0.5f + 0.5*texture(reflectionTexture, reflectionTexCoords).xyz;

	//fColor = (diff + a) * vec4(combinedColor, 1.0f);
	fColor = vec4(combinedColor, 1.0f);	
}