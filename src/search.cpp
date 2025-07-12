#include "../include/search.hpp"
#include "../include/types.hpp"
#include "../include/board.hpp"

namespace n_sch {
    using namespace n_brd;
    // using namespace n_ctr;
    // using namespace n_tmr;
    using namespace n_types;

    namespace {
        // typedef x functype;
        evl _negamax(board b, evl e_alpha, evl e_beta, u16 u_depth) {
            if (u_depth == 0) {
                return b.eval();
            }

            vector<cMove> v_moves = b.genLegalMoves();

            // break on i = 7
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
        return _negamax(b, INT64_MIN, INT64_MAX, MAXDEPTH);
    }
}
