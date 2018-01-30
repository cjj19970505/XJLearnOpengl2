#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec2 TexCoords[];
out vec2 TexCoord;
void main()
{
    gl_Position = gl_in[0].gl_Position;
	TexCoord = TexCoords[0];
	EmitVertex();
	
    gl_Position = gl_in[1].gl_Position;
	TexCoord = TexCoords[1];
	EmitVertex();
	
    gl_Position = gl_in[2].gl_Position;
	TexCoord = TexCoords[2];
    EmitVertex();
}