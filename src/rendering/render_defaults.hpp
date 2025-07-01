#ifndef _RENDER_DEFAULTS_HPP
#define _RENDER_DEFAULTS_HPP

#include "render_primitives.hpp"

#include "../helpers/static_expr.hpp"

constexpr ogl_color GL_BACKGROUND = {
  .r = uint8_to_float(0x27),
  .g = uint8_to_float(0x2a),
  .b = uint8_to_float(0x2e),
  .a = uint8_to_float(0xff)
};

#endif // _RENDER_DEFAULTS_HPP
