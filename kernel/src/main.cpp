#include <limine.h>


#include <oliveos/graphics/framebuffer.hpp>
#include <oliveos/graphics/text.hpp>
#include <oliveos/serial.hpp>

using namespace oliveos;

namespace {

    __attribute__((used, section(".limine_requests_start")))
    volatile uint64_t limine_requests_start_marker[] =
        LIMINE_REQUESTS_START_MARKER;

    __attribute__((used, section(".limine_requests")))
    volatile uint64_t limine_base_revision[] =
        LIMINE_BASE_REVISION(6);

    __attribute__((used, section(".limine_requests")))
    volatile limine_framebuffer_request framebuffer_request = {
        .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
        .revision = 0,
        .response = nullptr
    };

    __attribute__((used, section(".limine_requests_end")))
    volatile uint64_t limine_requests_end_marker[] =
        LIMINE_REQUESTS_END_MARKER;

    [[noreturn]] void halt_forever() {
        for (;;) {
            asm volatile("hlt");
        }
    }

}

extern "C" void kernel_main() {
    if (!LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision)) {
        halt_forever();
    }

    serial::initialize();
    serial::write("Welcome to OliveOS\n");

    if (framebuffer_request.response == nullptr
        || framebuffer_request.response->framebuffer_count == 0) {
        serial::write("Framebuffer unavailable\n");
        halt_forever();
        }

    const limine_framebuffer* framebuffer =
        framebuffer_request.response->framebuffers[0];

    if (framebuffer == nullptr
        || !framebuffer::is_supported(*framebuffer)) {
        serial::write(
            "Unsupported framebuffer format\n");

        halt_forever();
        }

    constexpr framebuffer::Color default_color = {
        .red = 59,
        .green = 74,
        .blue = 43
    };

    constexpr framebuffer::Color test_color = {
        .red = 100,
        .green = 20,
        .blue = 100
    };

    oliveos::framebuffer::clear(
        *framebuffer,
        default_color);

    serial::write("Framebuffer filled\n");

    framebuffer::fill_rectangle(*framebuffer, 32, 32, 100, 100,
        test_color);

    framebuffer::fill_rectangle(*framebuffer, 1200, 700, 100, 100,
        test_color);

    framebuffer::clear(*framebuffer, default_color);


    text::draw_character(*framebuffer, 'B', 100, 100, test_color, 20);

    text::draw_character(*framebuffer, 'a', 300, 100, test_color, 20);

    text::draw_character(*framebuffer,' ', 500, 100, test_color, 20);

    text::draw_character(*framebuffer, '3', 700, 100, test_color, 20);

    text::draw_string(*framebuffer, "Hello World!\nI love Gorbie!", 100, 300, test_color, 2);

    halt_forever();
}