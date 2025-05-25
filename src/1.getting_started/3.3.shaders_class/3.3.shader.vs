#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aCol;
out vec4 outColor;
void main()
{
	gl_Position = vec4(aPos, 1.0);
	outColor    = vec4(aCol, 1.0);
}