#include "types.hpp"
#include "board.hpp"
#include <functional>
#pragma once
namespace n_sch {
#define MAXDEPTH 4
    s64 search(n_brd::board b);
    s64 _negamax(n_brd::board b, s64 alpha, s64 beta, s16 depth);
    s64 _searchDepth(n_brd::board b, s16 depth);

    // search timer 
// max time set to value below (s)
#define MAXTIME 10 
    void _startTimer(reference_wrapper<bool> b_signal);
}
