#include "types.hpp"
#include "constants.hpp"

namespace n_bbd {

    // isolates the least significant bit
    constexpr bitboard bitIsolate(const bitboard bb) {
        return bb & -bb;
    }


    u8 bitPopCount(bitboard bb);
    
    // array<u8, 256> const _createPopLookup();
    constexpr bitboard shiftOne(const n_types::dir d, const bitboard bb) {
        const bitboard mask = n_consts::shiftMasks[n_consts::dirIndex(d)];
        if (d < 0) {
            return bb >> -d & mask;
        }
        return bb << d & mask;
    }
}
