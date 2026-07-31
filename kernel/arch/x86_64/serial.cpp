#include <oliveos/serial.hpp>

#include <stdint.h>

namespace oliveos::serial {
    namespace {

        constexpr uint16_t com1_port = 0x3F8;

        void write_port(uint16_t port, uint8_t value) {
            asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
        }

        uint8_t read_port(uint16_t port) {
            uint8_t value;
            asm volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
            return value;
        }

        bool can_transmit() {
            return (read_port(com1_port + 5) & 0x20) != 0;
        }

        void write_character(const char character) {
            while (!can_transmit()) {
            }

            write_port(com1_port, static_cast<uint8_t>(character));
        }

    }

    void initialize() {
        write_port(com1_port + 1, 0x00);
        write_port(com1_port + 3, 0x80);
        write_port(com1_port + 0, 0x03);
        write_port(com1_port + 1, 0x00);
        write_port(com1_port + 3, 0x03);
        write_port(com1_port + 2, 0xC7);
        write_port(com1_port + 4, 0x0B);
    }

    void write(const char* text) {
        while (*text != '\0') {
            if (*text == '\n') {
                write_character('\r');
            }

            write_character(*text);
            ++text;
        }
    }

}