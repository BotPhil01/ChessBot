#include "../include/sliding.hpp"

using namespace n_bbd;
using namespace n_types;
bitboard n_sld::lookup(const square bb_src, const bitboard bb_blockers, const piece p) {
    if (p == ROOK) {
        return _genRookAttack(bb_src, bb_blockers);
    } else if (p == BISHOP) {
        return _genBishopAttack(bb_src, bb_blockers);
    } else {
        return 0;
    }
}
