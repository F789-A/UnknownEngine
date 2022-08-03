#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 vertNormal;
layout (location = 2) in vec2 coord;

out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 model;

layout (std140, binding = 2) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0); 
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    vs_out.normal = normalize(vec3(projection * vec4(normalMatrix * vertNormal, 0.0)));
}