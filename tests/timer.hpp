#include "../include/timer.hpp"
#include "tConsts.hpp"
#include <thread>

using namespace std;
namespace n_tmr {
    bool dub() {
        bool ret = true;
        string msg = "TEST TIMER DUB ";
        start();
        ret = !start();
        tConsts::eval(ret, msg);
        reset();
        return ret;
    }
    
    bool gen() {
        bool ret = true;
        string msg = "TEST TIMER GEN ";
        if (b_timeUp != false) goto f;
        if (!start()) goto f;
        this_thread::sleep_for(chrono::seconds(STOPTIME / 2));
        if (b_timeUp != false) goto f;
        this_thread::sleep_for(chrono::seconds(STOPTIME / 2));
        if (b_timeUp != true) goto f;
s:
        ret = true;
        goto e;
f:
        ret = false;
e:
        tConsts::eval(ret, msg);
        reset();
        return ret;
    }

    bool tests() {
        return (gen() && dub());
    }
}
