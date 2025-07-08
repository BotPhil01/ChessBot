#include "types.hpp"
#include "board.hpp"
#pragma once
namespace n_sch {
    s64 search(n_brd::board b);
    s64 _negamax(n_brd::board b, s64 alpha, s64 beta, s16 depth);
    s64 _searchDepth(n_brd::board b, s16 depth);
}
