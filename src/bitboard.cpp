#include "../include/types.hpp"
#include "../include/bitboard.hpp"
#include <array>
#include <memory>
// bitboard functions
namespace n_bbd {
    // isolate a bit
    // get bit count
    // shift a bitboard by a 
    
    // array<u8, 256> const _createPopLookup() {
    //     array<u8, 256> a_ret = {{}};
    //     a_ret[0] = 0;
    //     for (int i = 0 ; i < 256; i++) {
    //         a_ret[i] = a_ret[i / 2] + (i & 1);
    //     }
    //     return a_ret;
    // }

    pShiftData pShift(
            const bitboard bb,
            const n_types::colour c,
            const square s_enPassantTgt,
            unique_ptr<ptrtype> &up_bbDfd) {
        const n_types::dir d = (c == white) ? n_types::NORTH : n_types::SOUTH;
        const bitboard bb_mask = (c == white) ? 0x0000000000FF0000 : 0x0000FF00000000000;

        // quiet moves
        const bitboard bb_shift = shiftOne(d, bb);
        const bitboard bb_2shift = shiftOne(d, bb_shift & bb_mask);

        // en passant
        bitboard bb_eepShift = 0;
        bitboard bb_wepShift = 0;

        if (shiftOne(n_types::EAST, bb) & s_enPassantTgt) {
            bb_eepShift = shiftOne(n_types::EAST, bb_shift);
        } else if (shiftOne(n_types::WEST, bb) & s_enPassantTgt) {
            bb_wepShift = shiftOne(n_types::WEST, bb_shift);
        }

        // attacks
        bitboard bb_blockers = 0;
        for (const bitboard bb : *up_bbDfd) {
            bb_blockers |= bb;
        }

        const bitboard bb_eShift = shiftOne(n_types::EAST, bb_shift) & bb_blockers;

        const bitboard bb_wShift = shiftOne(n_types::WEST, bb_shift) & bb_blockers;
        return {
            .s_fwd = bb_shift,
                .s_fwd2 = bb_2shift,
                .s_eCap = bb_eShift,
                .s_wCap = bb_wShift,
                .s_eep = bb_eepShift,
                .s_wep = bb_wepShift,
        };
    }

    u32 square2index(const square s) {
        square s_tmp = s;
        u32 i = 0;
        while (s_tmp != 1) {
            s_tmp = s_tmp >> 1;
            i++;
        }
        return i;
    }
}
