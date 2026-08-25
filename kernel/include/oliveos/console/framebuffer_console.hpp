#pragma once

#include <stdint.h>
#include <limine.h>
#include <oliveos/graphics/framebuffer.hpp>

namespace oliveos::console {

    struct FramebufferConsole {
        const limine_framebuffer* target;

        uint64_t origin_x;
        uint64_t origin_y;

        uint64_t cursor_x;
        uint64_t cursor_y;

        framebuffer::Color foreground;
        framebuffer::Color background;

        uint32_t scale;
    };

    bool init(
        FramebufferConsole& console,
        const limine_framebuffer& target,
        uint64_t origin_x,
        uint64_t origin_y,
        framebuffer::Color foreground,
        framebuffer::Color background,
        uint32_t scale
    );

    void clear(FramebufferConsole& console);
    void newline(FramebufferConsole& console);
    void put_char(FramebufferConsole& console, char c);
    void write(FramebufferConsole& console, const char* text);

}
