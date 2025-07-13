#include "types.hpp"
#include <array>

#pragma once
namespace n_rnd {
    extern u64 _state;
    u64 _genRand();

#define SRANDS (12 * 64)
    extern const array<u64, SRANDS> a_rands;
    array<u64, SRANDS> _initRands();
}
