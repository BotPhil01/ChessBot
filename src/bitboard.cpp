#include "../include/types.hpp"
#include <array>
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
}
