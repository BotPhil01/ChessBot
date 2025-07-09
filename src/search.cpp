#include "../include/search.hpp"
#include "../include/types.hpp"
#include <thread>
#include <cstdint>
#include <ctime>

using namespace n_sch;
using namespace n_types;
using namespace n_brd;

// redefine due to twos complement adding 1 extra number for negative 
// the above makes -INT64_MIN return INT64_MIN
// redefinition makes INT64_MAX the inverse of INT64_MIN under -
#undef INT64_MIN
#define INT64_MIN 0x8000000000000001

namespace {
    bool _b_timeUp = false;
}
s64 n_sch::search(board b) {
    jthread j_timer(_startTimer, ref(_b_timeUp));
    cout << "bool address: " << &::_b_timeUp << '\n';
    s64 ret = _negamax(b, INT64_MIN, INT64_MAX, MAXDEPTH);
    if (!::_b_timeUp) {
        cout << "n_sch::search(board b) finished before reaching maxdepth\n";
        if (j_timer.joinable()) j_timer.join();
        ::_b_timeUp = false;
    }
    return ret;
}

s64 n_sch::_negamax(board b, s64 s_alpha, s64 s_beta, s16 s_depth) {
    if (s_depth == 0) {
        // return quiesce();
        return b.eval();
    }
    const vector<cMove> v_moves = b.genLegalMoves();
    s64 s_bestEval = b.evalInit();
    u64 u_move;
    for (u_move = 0; u_move < v_moves.size() && !::_b_timeUp; u_move++) {
        const cMove m = v_moves[u_move];
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
    cout << "starting _searchDepth: " << s_depth << '\n';
    cout << "timeup: " << ::_b_timeUp << '\n';
    return _negamax(b, INT64_MIN, INT64_MAX, s_depth);
}

void n_sch::_startTimer(reference_wrapper<bool> b_signal) {
    cout << "timer started\n";
    cout << "bool address: " << &(b_signal.get()) << '\n';
    b_signal.get() = false;
    const time_t t_start = time(nullptr);
    time_t t_end = time(nullptr);
    while (difftime(t_end, t_start) < MAXTIME) {
        t_end = time(nullptr);
    }
    cout << "time up\n";
    b_signal.get() = true;
    // cout << b_signal << '\n';
}
