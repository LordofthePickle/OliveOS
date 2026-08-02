#include <oliveos/graphics/font.hpp>
#include "builtin_font_data.hpp"

namespace oliveos::font {

  const Glyph& glyph_for(const char character) {
    if (character >= 'A' && character <= 'Z') {
      return builtin::uppercase_letters[character - 'A'];
    }

    if (character >= 'a' && character <= 'z') {
      return builtin::uppercase_letters[character - 'a']; //TODO: Add lower case
    }

    if (character == ' ') {
      return builtin::space_glyph;
    }

    return builtin::unknown_glyph;
  }


}