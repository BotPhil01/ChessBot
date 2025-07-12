// #include "../include/magics.hpp"
#include "bitboard.hpp"
#include "board.hpp"
#include <sys/resource.h>
#include "search.hpp"
#include "timer.hpp"
#include "nodectr.hpp"
int main() {
    int sum = 0;
    sum += n_bb::tests();
    sum += n_b::tests();
    // sum += n_tmr::tests();
    sum += n_ctr::tests();
    sum += n_sch::tests();
}

