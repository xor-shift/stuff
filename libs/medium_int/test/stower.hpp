#pragma once

#include <stuff/core/stow.hpp>
#include <stuff/medium_int.hpp>

#define STOW_TAG(_type, _bits) "stown_medium_int_" #_type "_" #_bits

// clang-format off
#define STOW_GENERATOR(_type, _bits)        \
template struct stf::detail::stow_private<  \
  stf::medium_int<_bits, _type>,            \
  &stf::medium_int<_bits, _type>::m_repr,   \
  STOW_TAG(_type, _bits)                    \
>

#define STOWN_PTR(_type, _bits)                 \
stf::detail::get_stown_member_ptr<              \
    stf::medium_int<_bits, _type>,              \
    std::array<_type, mint_elem_ct<_type, _bits>> stf::medium_int<_bits, _type>::*, \
    STOW_TAG(_type, _bits)                      \
>::m_ptr

// clang-format on

STOW_GENERATOR(u8, 8);
STOW_GENERATOR(u8, 7);
STOW_GENERATOR(u8, 16);
STOW_GENERATOR(u8, 15);
STOW_GENERATOR(u8, 9);
STOW_GENERATOR(u16, 16);
STOW_GENERATOR(u16, 15);
