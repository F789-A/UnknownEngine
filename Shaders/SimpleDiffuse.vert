#version 420 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 coord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 tang;

out VS_OUT
{
    vec2 texCoords;
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
    //vs_out.normal = normalize(vec3(projection * vec4(normalMatrix * normal, 0.0)));
    vs_out.texCoords = coord;
}