#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in mat4 model;

layout (std140, binding = 0) uniform matrices {
    mat4 view;
    mat4 projection;
};

out vec3 color;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
    color = aColor;
}