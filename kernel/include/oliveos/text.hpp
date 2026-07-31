#pragma once

#include <limine.h>
#include <oliveos/font.hpp>
#include <stdint.h>

#include <oliveos/framebuffer.hpp>

namespace oliveos::text {

    void draw_glyph(const limine_framebuffer& target, uint64_t x, uint64_t y,
        const font::Glyph& glyph, framebuffer::Color color, uint32_t scale);

}