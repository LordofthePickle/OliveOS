#pragma once

#include <limine.h>
#include <stdint.h>

namespace oliveos::framebuffer {

    struct Color {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    };

    bool is_supported(const limine_framebuffer& framebuffer);
    void clear(const limine_framebuffer& framebuffer, Color color);

    void put_pixel(
        const limine_framebuffer& framebuffer,
        uint64_t x,
        uint64_t y,
        Color color
    );

    void fill_rectangle(
        const limine_framebuffer& framebuffer,
        uint64_t x,
        uint64_t y,
        uint64_t width,
        uint64_t height,
        Color color
    );

}

