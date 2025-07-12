#include "tConsts.hpp"
#include "../include/nodectr.hpp"
#include <thread>

namespace n_ctr {
    using namespace std;

    bool ret = false;
    string msg = "TEST CTR ";

    void inc() {
        nctr var;
        var++;
    }

    bool set() {
        msg = "TEST NCTR SET ";
        nctr var;
        u64 u = _au_val;
        var = 2;
        ret = _au_val == 2 && u == 0;
        var = 0;
        tConsts::eval(ret, msg);
        return ret;
    }

    bool pref() {
        msg = "TEST NCTR PREF ";
        nctr v;
        v = 0;
        ++v;
        thread t1(inc);
        t1.join();
        ret = (v == 2);
        v = 0;
        tConsts::eval(ret, msg);
        return ret;
    }

    bool posf() {
        msg = "TEST NCTR POSF ";
        nctr v;
        v++;
        thread t1(inc);
        t1.join();
        ret = (v == 2);
        v = 0;
        tConsts::eval(ret, msg);
        return ret;
    }

    bool pos() {
        msg = "TEST NCTR POS ";
        thread t1(inc);
        t1.join();
        nctr v;
        v++;
        ret = (v == 2);
        v = 0;
        tConsts::eval(ret, msg);
        return ret;
    }

    bool dub() {
        msg = "TEST NCTR POS ";
        thread t1(inc);
        thread t2(inc);
        t1.join();
        t2.join();
        nctr v;
        ret = (v == 2);
        tConsts::eval(ret, msg);
        return ret;
    }

    bool tests() {
        return 
            set() &&
            pref() &&
            posf() &&
            pos() &&
            dub();
    }
}
