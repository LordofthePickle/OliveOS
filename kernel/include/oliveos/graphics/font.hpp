#pragma once
#include <stdint.h>

namespace oliveos::font {

    struct Glyph {
        uint8_t rows[8];
    };

    const Glyph& glyph_for(char character);

    inline constexpr uint8_t glyph_width = 8;
    inline constexpr uint8_t glyph_height = 8;

}
