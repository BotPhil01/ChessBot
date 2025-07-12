#include "../include/search.hpp"
#include "../include/types.hpp"
#include "../include/board.hpp"
#include "../include/nodectr.hpp"

namespace n_sch {
    using namespace n_brd;
    using namespace n_ctr;
    // using namespace n_tmr;
    using namespace n_types;

    namespace {
        // typedef x functype;
        evl _negamax(board b, evl e_alpha, evl e_beta, u16 u_depth) {
#pragma optimize( "", off)
            n_ctr::au_ctr++;
#pragma optimize( "", on)
            if (u_depth == 0) {
                return b.eval();
            }

            vector<cMove> v_moves = b.genLegalMoves();

            for (u16 i = 0; i < v_moves.size(); i++) {
                const cMove m = v_moves[i];
                b.playMove(m);
                const evl e_cur = -_negamax(b, -e_beta, -e_alpha, u_depth-1);
                b.unPlayMove();

                if (e_cur >= e_beta) {
                    return e_beta;
                }

                if (e_cur > e_alpha) {
                    e_alpha = e_cur;
                }
            }
            return e_alpha;
        }
    }

    evl search(board b) {
        // ctr = 1000;
        evl e_ret = _negamax(b, INT64_MIN, INT64_MAX, MAXDEPTH);
        return e_ret;
    }
}
