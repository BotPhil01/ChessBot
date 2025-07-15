#include "../include/search.hpp"
#include "../include/types.hpp"
#include "../include/board.hpp"
#include "../include/transpo.hpp"

#include "../include/constants.hpp"
#include "../include/nodectr.hpp"

namespace n_sch {
    using namespace n_brd;
    using namespace n_ctr;
    // using namespace n_tmr;
    using namespace n_types;
    using namespace n_consts;

    namespace {
        table t;
        qTable t_q;
        entry _e_tmp;
#define QMAXDEPTH 100
        evl _quiesce(board b, evl e_alpha, evl e_beta, u32 u_depth) {
            au_ctr++;
            au_quiDepth = max((u64) au_quiDepth.load(), (u64) QMAXDEPTH - u_depth);

            // search quiesce table
            entry e = t_q.query(b.u_zob);
            if (e.sig != b.sig()) {
                e = e_empty;
            }
            u64 z_best = b.u_zob;
            cMove m_best = m_empty;
            bSig s_best = b.sig();
            const evl e_delta = 975;
            // standing pat
            evl e_best = b.eval();

            if (e.u_hash != e_empty.u_hash) {
                au_col++;
                if (e.n == node::PV) {
                    cout << "QPV\n";
                    return e.e;
                }
                if (e.n == node::DELTA &&
                        e_alpha >= INT64_MIN + e_delta &&
                        e.e < e_alpha - e_delta) {
                    cout << "QDE\n";
                    return e.e;
                }
                if (e.n == node::ALL && e.e <= e_alpha) {
                    cout << "QAL\n";
                    return e.e;
                }
                if (e.n == node::CUT &&
                        e.e >= e_beta) {
                    cout << "QCU\n";
                    return e.e;
                }
            }

            if (u_depth == 0 || e_best >= e_beta) {
                // if (u_depth == 0) cout << "depth 0\n";
                au_qBeta++;
                _e_tmp = {
                    z_best,
                    m_best,
                    0,
                    e_best,
                    node::CUT,
                    0,
                    s_best
                };
                t_q.add(_e_tmp);
                return e_beta;
            }

            // delta cutoff to save processing power
            if (e_alpha >= INT64_MIN + e_delta &&
                    e_best < e_alpha - e_delta) {
                au_qDelta++;
                _e_tmp = {
                    z_best,
                    m_best,
                    0,
                    e_alpha,
                    node::DELTA,
                    0,
                    s_best
                };
                t_q.add(_e_tmp);
                return e_alpha;
            }

            if (e_best > e_alpha) {
                e_alpha = e_best;
            }

            const vector<cMove> v_moves = b.genLegalMoves();

            bool b_alphaChanged = false;
            for (const cMove m : v_moves) {
                if (m.isQuiet()) continue;

                b.playMove(m);
                const evl e_cur = -_quiesce(b, -e_beta, -e_alpha, u_depth-1);
                const u64 z_cur = b.u_zob;
                const bSig s_cur = b.sig();
                b.unPlayMove();

                if (e_cur >= e_beta) {
                    _e_tmp = {
                        z_cur,
                        m,
                        0,
                        e_cur,
                        node::CUT,
                        0,
                        s_cur
                    };
                    t_q.add(_e_tmp);
                    return e_cur;
                }
                if (e_cur > e_best) {
                    e_best = e_cur;
                    z_best = z_cur;
                    m_best = m;
                    s_best = s_cur;
                    if (e_cur > e_alpha) {
                        b_alphaChanged = true;
                        e_alpha = e_cur;
                    }
                }
            }

            node n = b_alphaChanged ? node::PV : node::ALL;
            _e_tmp = {
                z_best,
                m_best,
                0,
                e_best,
                n,
                0,
                s_best
            };
            t_q.add(_e_tmp);
            return e_best;
        }

        pair<evl, cMove> _negamax(
                board b,
                evl e_alpha,
                evl e_beta,
                u16 u_depth,
                cMove m_suggestion = n_consts::m_empty) {
            n_ctr::au_ctr++;
            entry e = t.query(b.u_zob);
            if (e.sig != b.sig()) {
                e = e_empty;
            }
            if (e.u_hash != e_empty.u_hash && e.u_depth >= u_depth) {
                n_ctr::au_col++;
                if (e.n == node::PV) {
                    // cout << "PV cutoff\n";
                    return {e.e, e.m_best};
                }
                if (e.n == node::ALL && e.e <= e_alpha) {
                    // cout << "All cutoff\n";
                    return {e.e, e.m_best};
                }
                if (e.n == node::CUT && e.e >= e_beta) {
                    // cout << "Cut cutoff\n";
                    return {e.e, e.m_best};
                }
            }

            cMove m_best = n_consts::m_empty;
            evl e_best = INT64_MIN;
            u64 z_best = b.u_zob;
            bSig s_best = b.sig();
            if (u_depth == 0) {
                e_best = _quiesce(b, e_alpha, e_beta, QMAXDEPTH);
                _e_tmp = {
                    z_best,
                    m_best,
                    u_depth,
                    e_best,
                    node::PV,
                    0,
                    s_best
                };
                t.add(_e_tmp);
                return {
                    e_best,
                        n_consts::m_empty
                };
            }

            if (m_suggestion == m_empty) {
                m_suggestion = e.m_best;
            }

            bool b_alphaChanged = false;
            if (m_suggestion != n_consts::m_empty) {
                b.playMove(m_suggestion);
                e_best = -_negamax(b, -e_beta, -e_alpha, u_depth-1).first;
                m_best = m_suggestion;
                s_best = b.sig();
                b.unPlayMove();

                if (e_best >= e_beta) {
                    return {e_best, m_suggestion};
                }

                if (e_best > e_alpha) {
                    b_alphaChanged = true;
                    e_alpha = e_best;
                }
            }

            vector<cMove> v_moves = b.genLegalMoves();

            for (u16 i = 0; i < v_moves.size(); i++) {
                const cMove m = v_moves[i];
                b.playMove(m);
                const evl e_cur = -_negamax(b, -e_beta, -e_alpha, u_depth-1).first;
                const u64 z_cur = zobrist(b);
                const bSig s_cur = b.sig();
                b.unPlayMove();

                if (e_cur >= e_beta) {
                    _e_tmp = {
                        z_cur,
                        m,
                        u_depth,
                        e_cur,
                        CUT,
                        0,
                        s_best
                    };
                    t.add(_e_tmp);
                    return {e_cur, m};
                }
                // 20295809

                if (e_cur > e_best) {
                    e_best = e_cur;
                    m_best = m;
                    z_best = z_cur;
                    s_best = s_cur;
                    if (e_cur > e_alpha) {
                        b_alphaChanged = true;
                        e_alpha = e_cur;
                    }
                }
                b.assValid();
            }
            node n_tmp = node::PV;
            if (b_alphaChanged) {
                n_tmp = node::ALL;
            }
            _e_tmp = {
                z_best,
                m_best,
                u_depth,
                e_best,
                n_tmp,
                0,
                s_best
            };
            t.add(_e_tmp);
            return {e_best, m_best};
        }

        pair<evl, cMove> _itdeep(board b) {
            pair<evl, cMove> p_cur = {INT64_MIN, n_consts::m_empty};
            au_ctr = 0;
            uint64_t u_cum = au_ctr;
            uint64_t u_col = au_col;
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
                cout << "Nodes collision in it " <<
                    i << " " << au_col - u_col << "\n";
                cout << "Max quiesce beta " <<
                    i << " " << au_qBeta << "\n";
                cout << "Max quiesce delta " <<
                    i << " " << au_qDelta << "\n";
                cout << "Max quiesce depth reached " <<
                    i << " " << au_quiDepth  << endl;
                u_col = au_col;
                u_cum = au_ctr;
                p_cur = e_ret;
            }
            return p_cur;
        }
    }


    pair<evl, cMove> search(board b) {
        // ctr = 1000;
        // iterative deepening
        pair<evl, cMove> e_ret = _itdeep(b);
        return e_ret;
    }
}
