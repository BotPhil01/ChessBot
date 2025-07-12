#include "../include/timer.hpp"
#include "../include/types.hpp"
#include <ctime>
#include <thread>
namespace n_tmr {
    bool b_timeUp = false;
    bool _b_started = false;
    void _threadBody() {
        time_t t_start = time(nullptr);
        time_t t_end = t_start;
        // busy waiting
        while (difftime(t_end, t_start) < STOPTIME) {
            t_end = time(nullptr);
        }
        b_timeUp = true;
        // _b_started = false;
    }

    bool start() {
        if (_b_started) {
            cout << "timer already started\n";
            return false;
        }
        _b_started = true;
        b_timeUp = false;
        thread t = thread(_threadBody);
        // timer operates independently
        t.detach();
        return true;
    }

    // should only be used for tests/debugging
    void reset() {
        _b_started = false;
        b_timeUp = true;
    }
}
