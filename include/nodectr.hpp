// atomic node counter to count nodes searched by search algo
#include "types.hpp"
#include <atomic>

#pragma once
namespace n_ctr {
    extern au64 au_col;
    extern au64 au_ctr;
    extern au64 au_quiDepth;
    extern au64 au_qBeta;
    extern au64 au_qDelta;
}
