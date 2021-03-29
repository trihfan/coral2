#pragma once

#include "resources/Resource.h"
#include <ft2build.h>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include FT_FREETYPE_H

namespace coral
{
    struct FreetypeGlyphParams
    {
        std::string font;
        char character;
        int size;
    };

    struct FreetypeGlyph
    {
        glm::ivec2 size;
        glm::ivec2 bearing;
        int advance;
        std::vector<unsigned char> data;
    };

    class Freetype
    {
    public:
        static void init();
        static void release();

        static FreetypeGlyph getGlyph(const FreetypeGlyphParams& params);

    private:
        // Lib handle
        static FT_Library freetype;

        // Face by font
        static std::unordered_map<std::string, FT_Face> faces;

        // Get the face of the given font
        static FT_Face getFace(const std::string& font);
    };
}
