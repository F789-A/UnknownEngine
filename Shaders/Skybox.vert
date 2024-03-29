#version 420 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 model;

void main()
{
    TexCoords = aPos; // need flip Y here
    vec4 pos = model * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}  