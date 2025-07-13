#include "../include/rands.hpp"

namespace n_rnd {
    u64 _state = 2601736853;
    u64 _genRand() {
        u64 u = _state;
        u ^= u << 13;
        u ^= u >> 7;
        u ^= u << 17;
        _state = u;
        return u;
    }

    array<u64, SRANDS> _initRands() {
        array<u64, SRANDS> a_ret;
        for (u32 i = 0; i < SRANDS; i++) {
            a_ret[i] = _genRand();
        }
        return a_ret;
    }

    const array<u64, SRANDS> a_rands = _initRands();
}
