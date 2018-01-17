#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec2 TexCoords;
out vec3 worldPos;
out vec3 worldNormal;
out vec4 vertexColor;
struct App
{
	vec3 lightPos[3];
	vec4 lightColor[3];
	
	mat4 model;
	mat4 view;
	mat4 projection;
};
uniform App app;

void main()
{
    TexCoords = aTexCoords;
	worldPos = (app.model*vec4(aPos,1)).xyz;
	worldNormal = mat3(transpose(inverse(app.model))) * aNormal;
	vertexColor = vec4(aNormal, 1);
    gl_Position = app.projection * app.view * app.model * vec4(aPos, 1.0);
	//gl_Position = vec4(aPos, 1.0);
}