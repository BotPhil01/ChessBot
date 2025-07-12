#include "../include/search.hpp"
#include "../include/types.hpp"
#include "../include/board.hpp"
#include "../include/constants.hpp"
#include "../include/nodectr.hpp"

namespace n_sch {
    using namespace n_brd;
    using namespace n_ctr;
    // using namespace n_tmr;
    using namespace n_types;

    namespace {
        evl _quiesce(board b, evl e_alpha, evl e_beta) {
            // standing pat
            evl e_best = b.eval();

            if (e_best >= e_beta) {
                return e_beta;
            }
            if (e_best > e_alpha) {
                e_alpha = e_best;
            }

            const vector<cMove> v_moves = b.genLegalMoves();

            for (const cMove m : v_moves) {
                if (m.isQuiet()) continue;

                b.playMove(m);
                const evl e_cur = -_quiesce(b, -e_beta, -e_alpha);
                b.unPlayMove();

                if (e_cur >= e_beta) {
                    return e_cur;
                }
                if (e_cur > e_best) {
                    e_best = e_cur;
                    if (e_cur > e_alpha) {
                        e_alpha = e_cur;
                    }
                }
            }

            return e_best;
        }

        pair<evl, cMove> _negamax(
                board b,
                evl e_alpha,
                evl e_beta,
                u16 u_depth,
                const cMove m_suggestion = n_consts::m_empty) {
            cMove m_best = n_consts::m_empty;
            evl e_best = INT64_MIN;
            n_ctr::au_ctr++;
            if (u_depth == 0) {
                return {
                    _quiesce(b, e_alpha, e_beta),
                    // b.eval(),
                    n_consts::m_empty
                };
            }

            if (m_suggestion == n_consts::m_empty) {
                b.playMove(m_suggestion);
                e_best = -_negamax(b, -e_beta, -e_alpha, u_depth-1).first;
                m_best = m_suggestion;
                b.unPlayMove();

                if (e_best >= e_beta) {
                    return {e_best, m_suggestion};
                }

                if (e_best > e_alpha) {
                    e_alpha = e_best;
                }
            }

            vector<cMove> v_moves = b.genLegalMoves();

            for (u16 i = 0; i < v_moves.size(); i++) {
                const cMove m = v_moves[i];
                b.playMove(m);
                const evl e_cur = -_negamax(b, -e_beta, -e_alpha, u_depth-1).first;
                b.unPlayMove();

                if (e_cur >= e_beta) {
                    return {e_cur, m};
                }

                if (e_cur > e_best) {
                    e_best = e_cur;
                    m_best = m;
                    if (e_cur > e_alpha) {
                        e_alpha = e_cur;
                    }
                }
            }
            return {e_best, m_best};
        }

        evl _itdeep(board b) {
            pair<evl, cMove> p_cur = {INT64_MIN, n_consts::m_empty};
            au_ctr = 0;
            uint64_t u_cum = au_ctr;
            for (s16 i = 0; i < MAXDEPTH + 1; i++) {
                pair<evl, cMove> e_ret = _negamax(
                        b,
                        INT64_MIN,
                        INT64_MAX,
                        i,
                        p_cur.second
                        );
                cout << "Nodes searched in it " <<
                    i << " " << au_ctr - u_cum << "\n";
                u_cum = au_ctr;
                p_cur = e_ret;
            }
            return p_cur.first;
        }
    }


    evl search(board b) {
        // ctr = 1000;
        // iterative deepening
        evl e_ret = _itdeep(b);
        return e_ret;
    }
}
