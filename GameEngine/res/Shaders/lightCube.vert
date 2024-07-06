#version 420 core
layout (location = 0) in vec3 aPos;

layout (std140, binding = 0) uniform matrices {
                        // Size     Offset
    mat4 view;          // 16 * 4   0
    mat4 projection;    // 16 * 4   16 * 4 = 64
};  // Size = (16 * 4) * 2 = 128 bytes

uniform mat4 model;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}