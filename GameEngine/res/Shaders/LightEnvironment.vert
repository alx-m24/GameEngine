#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 model;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 FragPosLightSpace;

layout (std140, binding = 0) uniform matrices {
    mat4 view;
    mat4 projection;
};

uniform mat4 lightSpaceMatrix;

void main()
{
    Normal = mat3(transpose(inverse(model))) * aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0f));
    TexCoords = aTexCoord;
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0f);

    gl_Position = projection * view * model * vec4(aPos, 1.0);
} 