#version 420 core
out vec4 FragColor;

uniform sampler2D diffuse;

in VS_OUT
{
    vec2 texCoords;
} vs_in;

void main()
{  
    FragColor = vec4(0, 0, 0, 1);
}