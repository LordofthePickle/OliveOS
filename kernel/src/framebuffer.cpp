#include <oliveos/framebuffer.hpp>

namespace oliveos::framebuffer {
    namespace {

        bool channel_is_supported(const uint8_t size, const uint8_t shift) {
            return size > 0 && size < 32 && size + shift <= 32;
        }

        uint32_t scale_channel(const uint8_t value, const uint8_t size) {
            const uint32_t maximum =
                (static_cast<uint32_t>(1) << size) - 1;

            return static_cast<uint32_t>(value) * maximum / 255;
        }

        uint32_t encode_color(
            const limine_framebuffer& framebuffer,
            const Color color
        ) {
            const uint32_t red =
                scale_channel(color.red, framebuffer.red_mask_size)
                << framebuffer.red_mask_shift;

            const uint32_t green =
                scale_channel(color.green, framebuffer.green_mask_size)
                << framebuffer.green_mask_shift;

            const uint32_t blue =
                scale_channel(color.blue, framebuffer.blue_mask_size)
                << framebuffer.blue_mask_shift;

            return red | green | blue;
        }

    }

    bool is_supported(const limine_framebuffer& framebuffer) {
        return framebuffer.address != nullptr
            && framebuffer.bpp == 32
            && framebuffer.memory_model == LIMINE_FRAMEBUFFER_RGB
            && channel_is_supported(
                framebuffer.red_mask_size,
                framebuffer.red_mask_shift)
            && channel_is_supported(
                framebuffer.green_mask_size,
                framebuffer.green_mask_shift)
            && channel_is_supported(
                framebuffer.blue_mask_size,
                framebuffer.blue_mask_shift);
    }

    void clear(const limine_framebuffer& framebuffer, const Color color) {
        const uint32_t encoded_color =
            encode_color(framebuffer, color);

        auto* address = static_cast<uint8_t*>(framebuffer.address);

        for (uint64_t y = 0; y < framebuffer.height; ++y) {
            auto* row = reinterpret_cast<volatile uint32_t*>(
                address + y * framebuffer.pitch);

            for (uint64_t x = 0; x < framebuffer.width; ++x) {
                row[x] = encoded_color;
            }
        }
    }

    void put_pixel(const limine_framebuffer& framebuffer, const uint64_t x, const uint64_t y, const Color color) {

        if (x >= framebuffer.width || y >= framebuffer.height) return;

        const uint32_t encoded_color = encode_color(framebuffer, color);

        auto* address = static_cast<uint8_t*>(framebuffer.address);
        auto* row = reinterpret_cast<volatile uint32_t*>(address + y * framebuffer.pitch);

        row[x] = encoded_color;
    }

    void fill_rectangle(const limine_framebuffer& framebuffer,
        const uint64_t x, const uint64_t y, uint64_t width, uint64_t height, const Color color) {

        if (x >= framebuffer.width || y >= framebuffer.height) return;
        if (width > framebuffer.width - x) {
            width = framebuffer.width - x;
        }
        if (height > framebuffer.height - y) {
            height = framebuffer.height - y;
        }

        for (uint64_t i = 0; i < height; ++i) {
            for (uint64_t j = 0; j < width; ++j) {
                put_pixel(framebuffer, x + j, y + i, color);
            }
        }


    }

}