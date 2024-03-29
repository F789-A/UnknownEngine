#version 420 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 coord;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(pos, 1.0);
    TexCoords = coord;
}