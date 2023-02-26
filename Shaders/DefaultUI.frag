#version 420 core
out vec4 FragColor;

uniform sampler2D diffuse;

in VS_OUT
{
    vec2 texCoords;
} vs_in;

void main()
{  
    FragColor = vec4(vec3(texture(diffuse, vs_in.texCoords)), 1);
    //FragColor = vec4(vs_in.texCoords, 0, 1);
}