#version 420 core
out vec4 FragColor;

uniform sampler2D diffuse;

in VS_OUT
{
    vec2 texCoords;
} vs_in;

void main()
{  
    vec4 col = texture(diffuse, vs_in.texCoords);
    if (col.a < 0.01f)
    {
        discard;
    }
    FragColor = vec4(vec3(col), 1.0f);
}