#include "types.hpp"
#pragma once
namespace n_sld {
    bitboard lookup(const bitboard bb_src, const  bitboard bb_blockers, const n_types::piece p);
};
