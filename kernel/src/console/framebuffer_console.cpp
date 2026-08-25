#include <oliveos/console/framebuffer_console.hpp>
#include <oliveos/graphics/font.hpp>
#include <oliveos/graphics/text.hpp>

namespace {
    uint64_t get_glyph_pixel_width(const uint32_t scale) {
        return oliveos::font::glyph_width * scale;
    }

    uint64_t get_glyph_pixel_height(const uint32_t scale) {
        return oliveos::font::glyph_height * scale;
    }

    uint64_t get_cell_width(const uint32_t scale) {
        return (oliveos::font::glyph_width + 1) * scale;
    }

    uint64_t get_cell_height(const uint32_t scale) {
        return (oliveos::font::glyph_height + 1) * scale;
    }

    bool check_init(const oliveos::console::FramebufferConsole& console) {
        if (console.target == nullptr) {
            return false;
        }
        return true;
    }
}

namespace oliveos::console {
    bool init(FramebufferConsole& console, const limine_framebuffer& target,
        const uint64_t origin_x, const uint64_t origin_y, const framebuffer::Color foreground,
        const framebuffer::Color background, const uint32_t scale) {

        if (scale == 0) {
            return false;
        }

        if (origin_x >= target.width || origin_y >= target.height) {
            return false;
        }

        console.target = &target;
        console.origin_x = origin_x;
        console.origin_y = origin_y;
        console.cursor_x = origin_x;
        console.cursor_y = origin_y;
        console.foreground = foreground;
        console.background = background;
        console.scale = scale;

        return true;
    }

    void clear(FramebufferConsole& console) {

        if (!check_init(console)) {
            return;
        }

        framebuffer::clear(*console.target, console.background);
        console.cursor_x = console.origin_x;
        console.cursor_y = console.origin_y;

    }

    void newline(FramebufferConsole& console) {

        if (!check_init(console)) {
            return;
        }

        console.cursor_x = console.origin_x;
        console.cursor_y += get_cell_height(console.scale);

        if (console.cursor_y + get_cell_height(console.scale) > console.target->height) {
            clear(console);
        }

    }

    void put_char(FramebufferConsole& console, const char c) {

        if (!check_init(console)) {
            return;
        }

        switch (c) {
            case '\0': return;
            case '\n': newline(console);
                break;
            default: {
                if (console.cursor_x + get_glyph_pixel_width(console.scale) > console.target->width) {
                    newline(console);
                }
                text::draw_character(*console.target, c, console.cursor_x, console.cursor_y, console.foreground, console.scale);
                console.cursor_x += get_glyph_pixel_width(console.scale);

            };
        }

    }

    void write(FramebufferConsole& console, const char* text) {
        if (!check_init(console)) {
            return;
        }
        if (text == nullptr) {
            return;
        }

        while (*text != '\0') {
            put_char(console, *text);
            if (console.cursor_x + get_glyph_pixel_width(console.scale) > console.target->width) {
                newline(console);
            }
            else {
                console.cursor_x += get_glyph_pixel_width(console.scale);
            }
            ++text;
        }
    }

}
