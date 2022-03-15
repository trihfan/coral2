#include "Freetype.h"
#include "utils/Logs.h"
#include "resources/Resource.h"

using namespace coral;

Freetype::Freetype()
{
    if (FT_Init_FreeType(&freetype))
    {
        Logs(error) << "Could not init FreeType Library";
    }
}

Freetype::~Freetype()
{
    for (const auto& pair : faces)
    {
        FT_Done_Face(pair.second);
    }
    FT_Done_FreeType(freetype);
}

FT_Face Freetype::getFace(const std::string& font)
{
    auto it = faces.find(font);
    if (it != faces.end())
    {
        return it->second;
    }

    FT_Face face;
    if (FT_New_Face(freetype, font.c_str(), 0, &face))
    {
        Logs(error) << "Failed to load font";
        return nullptr;
    }

    faces[font] = face;
    return face;
}

FreetypeGlyph Freetype::getGlyph(const FreetypeGlyphParams& params)
{
    FreetypeGlyph glyph;
    FT_Face face = getFace(params.font);
    FT_Set_Pixel_Sizes(face, 0, static_cast<unsigned int>(params.size));

    // Load character glyph
    if (FT_Load_Char(face, static_cast<unsigned char>(params.character), FT_LOAD_RENDER))
    {
        Logs(error) << "Failed to load Glyph";
    }

    glyph.data.resize(face->glyph->bitmap.width * face->glyph->bitmap.rows);
    std::memcpy(glyph.data.data(), face->glyph->bitmap.buffer, glyph.data.size());
    glyph.size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
    glyph.bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
    glyph.advance = static_cast<int>(face->glyph->advance.x);

    return glyph;
}
