#pragma once

#include <limine.h>
#include <oliveos/graphics/font.hpp>
#include <stdint.h>

#include <oliveos/graphics/framebuffer.hpp>

namespace oliveos::text {

    void draw_glyph(const limine_framebuffer& target, uint64_t x, uint64_t y,
        const font::Glyph& glyph, framebuffer::Color color, uint32_t scale);

    void draw_character(const limine_framebuffer& target, char character, uint64_t x, uint64_t y,
        framebuffer::Color color, uint32_t scale);

    void draw_string(const limine_framebuffer& target, const char* text, uint64_t x, uint64_t y,
        framebuffer::Color color, uint32_t scale);

}