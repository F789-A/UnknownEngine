#version 420 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 coord;

out VS_OUT
{
    vec2 texCoords;
} vs_out;

uniform mat4 model;


void main()
{
    gl_Position = model * vec4(position, 1.0); 
    vs_out.texCoords = vec2(coord.x, 1-coord.y);
}