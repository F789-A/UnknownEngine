#version 420 core
out vec4 FragColor;
  
uniform sampler2D screenTexture;

in vec2 texCoord;

layout (std140, binding = 3) uniform General
{
    int aspectX;
    int aspectY;
};

uniform int intencity;

uniform int bloor = 0;

void main()
{
    float size = 1;
    if (aspectX > aspectY)
    {
        size = float(aspectX ) / float(aspectY) * intencity;
    }
    else
    {
        size = float(aspectY) / float(aspectX ) * intencity;
    }
    vec2 newCoord =  round(texCoord * size) / size;
    vec4 result =  vec4(texture(screenTexture, newCoord));

    if (bloor == 1)
    {
        size = 1/size;
        result +=  vec4(texture(screenTexture, newCoord+vec2(size/2, 0)));
        result +=  vec4(texture(screenTexture, newCoord+vec2(size/2, size/2)));
        result +=  vec4(texture(screenTexture, newCoord+vec2(0, size/2)));
        result +=  vec4(texture(screenTexture, newCoord+vec2(-size/2, size/2)));
        result +=  vec4(texture(screenTexture, newCoord+vec2(-size/2, 0)));
        result +=  vec4(texture(screenTexture, newCoord+vec2(-size/2, -size/2)));
        result +=  vec4(texture(screenTexture, newCoord+vec2(0, -size/2)));
        result +=  vec4(texture(screenTexture, newCoord+vec2(size/2, -size/2)));
        result /= 9;
    }

    FragColor = result;
}