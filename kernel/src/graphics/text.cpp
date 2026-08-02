#include <oliveos/graphics/text.hpp>
#include <oliveos/graphics/font.hpp>

namespace oliveos::text {
    constexpr uint8_t masks[] = {
        0b10000000,
        0b01000000,
        0b00100000,
        0b00010000,
        0b00001000,
        0b00000100,
        0b00000010,
        0b00000001
    };

    void draw_glyph(const limine_framebuffer& framebuffer, const uint64_t x, const uint64_t y,
    const font::Glyph& glyph, const framebuffer::Color color, const uint32_t scale) {

        if (scale == 0) {
            return;
        }

        for (int64_t row = 0; row < font::glyph_height; row++) {
            for (int64_t col = 0; col < font::glyph_width; col++) {
                if ((masks[col] & glyph.rows[row]) != 0) {
                    const uint64_t pixel_x = x + col * scale;
                    const uint64_t pixel_y = y + row * scale;
                    const uint64_t pixel_width = scale;
                    const uint64_t pixel_height = scale;

                    framebuffer::fill_rectangle(framebuffer, pixel_x, pixel_y, pixel_width, pixel_height, color);
                }
            }
        }

    }

    void draw_character(const limine_framebuffer& framebuffer, const char character,
        const uint64_t x, const uint64_t y, const framebuffer::Color color, const uint32_t scale) {

        const font::Glyph& glyph = font::glyph_for(character);

        draw_glyph(framebuffer, x, y, glyph, color, scale);
    }

    void draw_string(const limine_framebuffer& framebuffer, const char* text, const uint64_t x, const uint64_t y,
        const framebuffer::Color color, const uint32_t scale) {

        uint64_t cursor_x = x;
        uint64_t cursor_y = y;
        uint64_t index = 0;
        const uint64_t hor_advance = font::glyph_width * scale + 1;
        const uint64_t ver_advance   = font::glyph_height * scale + 1;

        if (text == nullptr || text[0] == '\0' || scale == 0) {
            return;
        }

        while (text[index] != '\0') {
            if (const char curr_character = text[index]; curr_character == '\n') {
                cursor_x = x;
                cursor_y += ver_advance;
            }
            else {
                draw_character(framebuffer, curr_character, cursor_x, cursor_y, color, scale);
                cursor_x += hor_advance;
            }
            index++;
        }

    }
}





