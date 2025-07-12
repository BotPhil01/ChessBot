// timer to interrupt search
#pragma once

namespace n_tmr {
#define STOPTIME 60
    extern bool b_timeUp;
    bool start();
    void reset();
}
