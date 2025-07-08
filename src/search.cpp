#include "../include/search.hpp"
#include "../include/types.hpp"
#include <cstdint>

using namespace n_sch;
using namespace n_types;
using namespace n_brd;

#define DEPTH 2
// redefine due to twos complement adding 1 extra number for negative 
// the above makes -INT64_MIN return INT64_MIN
// redefinition makes INT64_MAX the inverse of INT64_MIN under -
#undef INT64_MIN
#define INT64_MIN 0x8000000000000001
s64 n_sch::search(board b) {
    return _negamax(b, INT64_MIN, INT64_MAX, DEPTH);
}

s64 n_sch::_negamax(board b, s64 s_alpha, s64 s_beta, s16 s_depth) {
    if (s_depth == 0) {
        // return quiesce();
        return b.eval();
    }
    const vector<cMove> v_moves = b.genLegalMoves();
    s64 s_bestEval = b.evalInit();
    for (const cMove m : v_moves) {
        // search
        b.playMove(m);
        s64 s_curEval = -_negamax(b, -s_beta, -s_alpha, s_depth-1);
        if (s_curEval >= s_beta) {
            b.unPlayMove();
            return s_beta;
        }
        if (s_curEval > s_alpha) {
            s_alpha = s_curEval;
        }
        b.unPlayMove();
    }
    return s_alpha;
}
s64 n_sch::_searchDepth(n_brd::board b, s16 s_depth) {
    return _negamax(b, INT64_MIN, INT64_MAX, s_depth);
}
