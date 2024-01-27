#pragma once
#include <vector>
#include <map>
#include <iostream>
#include <glm/glm.hpp>
#include "GLTexture.h"
#include "Assets\Texture.h"

#include <ft2build.h>
#include FT_FREETYPE_H


using uint = unsigned int;

struct Character
{
    glm::ivec2 Size;        
    glm::ivec2 Coord;       
    glm::ivec2 Bearing;
    uint Advance;
};

class FontInfo
{
public:
    GLTexture texture;
    std::map<char, Character> Characters;
    int FontSize;

    FontInfo(){}

    FontInfo(const std::string& path, int fontSize)
    {
        FontSize = fontSize;
        FT_Library ft;
        if (FT_Init_FreeType(&ft))
        {
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        }
        FT_Face face;
        if (FT_New_Face(ft, path.c_str(), 0, &face))
        {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        }
        FT_Set_Pixel_Sizes(face, 0, fontSize);
        

        Texture text(512, 512, 1);
        glm::ivec2 cur(0, 0);

        for (GLubyte c = 0; c < 128; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            int width = face->glyph->bitmap.width;
            int rows = face->glyph->bitmap.rows;
            if (512 - cur.x < width)
            {
                cur.x = 0;
                cur.y += 48;
            }
            for (int y = 0; y < rows; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    text.set(cur.x + x, cur.y + y, face->glyph->bitmap.buffer[y * width + x]);
                }
            }
            Character character = 
            {
                glm::ivec2(width, rows),
                glm::ivec2(cur.x,  cur.y),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                (face->glyph->advance.x >> 6)
            };
            Characters.insert(std::pair<GLchar, Character>(c, character));
            cur.x += width;
        }
        texture = GLTexture(text);
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }

};