#include <oliveos/text.hpp>
#include <oliveos/font.hpp>

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

        for (int8_t i = 0; i < 8; i++) {
            for (int8_t j = 0; j < 8; j++) {
                if ((masks[j] & glyph.rows[i]) != 0) {
                    const uint64_t pixel_x = x + j * scale;
                    const uint64_t pixel_y = y + i * scale;
                    const uint64_t pixel_width = scale;
                    const uint64_t pixel_height = scale;

                    framebuffer::fill_rectangle(framebuffer, pixel_x, pixel_y, pixel_width, pixel_height, color);
                }
            }
        }

    }
}





