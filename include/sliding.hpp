#include "types.hpp"
#include "bitboard.hpp"
#include <array>

#pragma once
namespace n_sld {
    bitboard lookup(const bitboard bb_src, const  bitboard bb_blockers, const n_types::piece p);

    constexpr bitboard _genBishopAttack(const square s, const bitboard bb_blockers) {
        const array<n_types::dir, 4> a_dirs = {
            n_types::NORTHEAST,
            n_types::NORTHWEST,
            n_types::SOUTHEAST,
            n_types::SOUTHWEST
        };
        bitboard bb_ret = 0;
        for (const n_types::dir d : a_dirs) {
            // keep shifting until its the same as the previous or a blocker is intersected
            bitboard bb_shifts = 0;
            bitboard bb_shifted = n_bbd::shiftOne(d, s); 
            while (bb_shifted) {
                bb_shifts |= bb_shifted;
                if (bb_shifted & bb_blockers) {
                    break;
                }
                bb_shifted = n_bbd::shiftOne(d, bb_shifted);
            }
            bb_ret |= bb_shifts;
        }
        return bb_ret;
    }

    constexpr bitboard _genRookAttack(const square s, const bitboard bb_blockers) {
        const array<n_types::dir, 4> a_dirs = {
            n_types::NORTH,
            n_types::EAST,
            n_types::SOUTH,
            n_types::WEST
        };
        bitboard bb_ret = 0;
        for (const n_types::dir d : a_dirs) {
            // keep shifting until its the same as the previous or a blocker is intersected
            bitboard bb_shifts = 0;
            bitboard bb_shifted = n_bbd::shiftOne(d, s); 
            while (bb_shifted) {
                bb_shifts |= bb_shifted;
                if (bb_shifted & bb_blockers) {
                    break;
                }
                bb_shifted = n_bbd::shiftOne(d, bb_shifted);
            }
            bb_ret |= bb_shifts;
        }
        return bb_ret;
    }

};
