#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 worldPos;
in vec3 worldNormal;

in vec4 vertexColor;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
struct App
{
	vec3 lightPos[3];
	vec4 lightColor[3];
	
	mat4 model;
	mat4 view;
	mat4 projection;
};
uniform App app;
float LinearizeDepth(float depth, float near, float far) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));    
}
void main()
{
	FragColor = (vertexColor + vec4(1,1,1,1))/2;
}
