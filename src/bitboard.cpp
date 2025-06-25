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

    u8 bitPopCount(const bitboard bb) {
        // generated with _createPopLoookup
        const array<u8, 256> a_popLookup = {
            0, 1, 1, 2, 1, 2, 2, 3,
            1, 2, 2, 3, 2, 3, 3, 4,
            1, 2, 2, 3, 2, 3, 3, 4,
            2, 3, 3, 4, 3, 4, 4, 5,
            1, 2, 2, 3, 2, 3, 3, 4,
            2, 3, 3, 4, 3, 4, 4, 5,
            2, 3, 3, 4, 3, 4, 4, 5,
            3, 4, 4, 5, 4, 5, 5, 6,
            1, 2, 2, 3, 2, 3, 3, 4,
            2, 3, 3, 4, 3, 4, 4, 5,
            2, 3, 3, 4, 3, 4, 4, 5,
            3, 4, 4, 5, 4, 5, 5, 6,
            2, 3, 3, 4, 3, 4, 4, 5,
            3, 4, 4, 5, 4, 5, 5, 6,
            3, 4, 4, 5, 4, 5, 5, 6,
            4, 5, 5, 6, 5, 6, 6, 7,
            1, 2, 2, 3, 2, 3, 3, 4,
            2, 3, 3, 4, 3, 4, 4, 5,
            2, 3, 3, 4, 3, 4, 4, 5,
            3, 4, 4, 5, 4, 5, 5, 6,
            2, 3, 3, 4, 3, 4, 4, 5,
            3, 4, 4, 5, 4, 5, 5, 6,
            3, 4, 4, 5, 4, 5, 5, 6,
            4, 5, 5, 6, 5, 6, 6, 7,
            2, 3, 3, 4, 3, 4, 4, 5,
            3, 4, 4, 5, 4, 5, 5, 6,
            3, 4, 4, 5, 4, 5, 5, 6,
            4, 5, 5, 6, 5, 6, 6, 7,
            3, 4, 4, 5, 4, 5, 5, 6,
            4, 5, 5, 6, 5, 6, 6, 7,
            4, 5, 5, 6, 5, 6, 6, 7,
            5, 6, 6, 7, 6, 7, 7, 8, 
        };
        u8 *p_bb = (u8 *) &bb;
        return 
            a_popLookup[p_bb[0]] + 
            a_popLookup[p_bb[1]] + 
            a_popLookup[p_bb[2]] + 
            a_popLookup[p_bb[3]] + 
            a_popLookup[p_bb[4]] + 
            a_popLookup[p_bb[5]] + 
            a_popLookup[p_bb[6]] + 
            a_popLookup[p_bb[7]];
    }

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
}
