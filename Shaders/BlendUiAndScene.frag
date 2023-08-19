#version 420 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D sceneTexture;
uniform sampler2D uiTexture;

void main()
{
    vec4 ui = texture(uiTexture, TexCoords);
    if (ui.a > 0.5)
    {
        FragColor = ui;
    }
    else
    {
        FragColor = texture(sceneTexture, TexCoords);
    }
}