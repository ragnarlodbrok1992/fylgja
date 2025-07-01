#ifndef _STATIC_EXPR_HPP
#define _STATIC_EXPR_HPP

constexpr float uint8_to_float(uint8_t value) {
  return static_cast<float>(value) / 0xff;
}

#endif // _STATIC_EXPR_HPP
